/**
 * @file src/cxx/database/src/Arrayset.cc
 * @author <a href="mailto:Laurent.El-Shafey@idiap.ch">Laurent El Shafey</a>
 *
 * @brief A torch representation of an Array for a Dataset.
 */

#include "database/Arrayset.h"

namespace db = Torch::database;

db::Arrayset::Arrayset ():
  m_inlined(new db::detail::InlinedArraysetImpl()),
  m_external(),
  m_role("")
{
}

db::Arrayset::Arrayset (const db::detail::InlinedArraysetImpl& inlined):
  m_inlined(new db::detail::InlinedArraysetImpl(inlined)),
  m_external(),
  m_role("")
{
}

db::Arrayset::Arrayset(const std::string& filename, const std::string& codec):
  m_inlined(),
  m_external(new db::detail::ExternalArraysetImpl(filename, codec)),
  m_role("")
{
}

db::Arrayset::Arrayset(const db::Arrayset::Arrayset& other):
  m_inlined(other.m_inlined),
  m_external(other.m_external),
  m_role(other.m_role)
{
}

db::Arrayset::~Arrayset() {
}

db::Arrayset& db::Arrayset::operator= (const db::Arrayset& other) {
  m_inlined = other.m_inlined;
  m_external = other.m_external;
  m_role = other.m_role;
  return *this;
}

size_t db::Arrayset::add (boost::shared_ptr<const db::Array> array) {
  return add(*array.get());
}

size_t db::Arrayset::add (const db::Array& array) {
  if (m_inlined) return m_inlined->add(array);
  else return m_external->add(array);
}

size_t db::Arrayset::add (const db::detail::InlinedArrayImpl& array) {
  if (m_inlined) return m_inlined->add(array);
  else return m_external->add(array);
}

size_t db::Arrayset::add (const std::string& filename, const std::string& codec) {
  if (m_inlined) return m_inlined->add(db::Array(filename, codec));
  else return m_external->add(db::Array(filename, codec));
}

void db::Arrayset::add (size_t id, boost::shared_ptr<const db::Array> array) {
  add(id, *array.get());
}

void db::Arrayset::add (size_t id, const db::Array& array) {
  if (m_inlined) m_inlined->add(id, array);
  else m_external->add(id, array);
}

void db::Arrayset::add (size_t id, const db::detail::InlinedArrayImpl& array) {
  if (m_inlined) m_inlined->add(id, array);
  else m_external->add(id, array);
}

void db::Arrayset::add (size_t id, const std::string& filename, const std::string& codec) {
  if (m_inlined) m_inlined->add(id, db::Array(filename, codec));
  else m_external->add(id, db::Array(filename, codec));
}

void db::Arrayset::set (size_t id, boost::shared_ptr<const db::Array> array) {
  set(id, *array.get());
}

void db::Arrayset::set (size_t id, const db::Array& array) {
  if (m_inlined) (*m_inlined)[id] = array;
  else m_external->set(id, array);
}

void db::Arrayset::set (size_t id, const db::detail::InlinedArrayImpl& array) {
  if (m_inlined) (*m_inlined)[id] = array;
  else m_external->set(id, array);
}

void db::Arrayset::set (size_t id, const std::string& filename, const std::string& codec) {
  if (m_inlined) (*m_inlined)[id] = db::Array(filename, codec);
  else m_external->set(id, db::Array(filename, codec));
}

void db::Arrayset::remove (const size_t id) {
  if (m_inlined) m_inlined->remove(id);
  else m_external->remove(id);
}

Torch::core::array::ElementType db::Arrayset::getElementType() const {
  if (m_inlined) return m_inlined->getElementType();
  return m_external->getElementType();
}

size_t db::Arrayset::getNDim() const {
  if (m_inlined) return m_inlined->getNDim();
  return m_external->getNDim();
}

const size_t* db::Arrayset::getShape() const {
  if (m_inlined) return m_inlined->getShape();
  return m_external->getShape();
}

size_t db::Arrayset::getNSamples() const {
  if (m_inlined) return m_inlined->getNSamples();
  return m_external->getNSamples();
}

void db::Arrayset::save(const std::string& filename, const std::string& codecname) {
  if (m_inlined) {
    m_external.reset(new db::detail::ExternalArraysetImpl(filename, codecname, true));
    m_external->set(*m_inlined);
    m_inlined.reset();
    return;
  }
  m_external->move(filename, codecname); 
}

const std::string& db::Arrayset::getFilename() const {
  if (m_external) return m_external->getFilename();
  static std::string empty_string;
  return empty_string;
}

boost::shared_ptr<const db::ArraysetCodec> db::Arrayset::getCodec() const {
  if (m_external) return m_external->getCodec();
  return boost::shared_ptr<ArraysetCodec>(); 
}
    
void db::Arrayset::load() {
  if (!m_inlined) {
    m_inlined.reset(new detail::InlinedArraysetImpl(m_external->get()));
    m_external.reset();
  }
}

const db::Array db::Arrayset::operator[] (size_t id) const {
  if (m_inlined) return (*m_inlined)[id];
  return (*m_external)[id];
}

db::Array db::Arrayset::operator[] (size_t id) {
  if (m_inlined) return (*m_inlined)[id];
  return (*m_external)[id];
}

bool db::Arrayset::exists (size_t id) const {
  if (m_inlined) return m_inlined->exists(id);
  return m_external->exists(id);
}

void db::Arrayset::consolidateIds () {
  if (m_inlined) m_inlined->consolidateIds();
}

db::detail::InlinedArraysetImpl db::Arrayset::get() const {
  if (!m_inlined) return m_external->get();
  return *m_inlined.get();
}