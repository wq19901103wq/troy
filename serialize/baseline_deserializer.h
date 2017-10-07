#ifndef SERIALIZE_BASELINE_DESERIALIZER_H
#define SERIALIZE_BASELINE_DESERIALIZER_H

#include "serialize/deserializer.h"

// Baseline实现
class BaselineDeserializer : public Deserializer {
 public:
  BaselineDeserializer() = delete;
  BaselineDeserializer(std::istream& is): Deserializer(is) {}
  // virtual 不支持模板, 需要全部列出
  virtual bool Deserialize(char& s) override {
    is_.read(reinterpret_cast<char*>(&s), sizeof(s));
    return is_.good();
  }
  virtual bool Deserialize(int16_t& s) override {
    is_.read(reinterpret_cast<char*>(&s), sizeof(s));
    return is_.good();
  }
  virtual bool Deserialize(uint16_t& s) override {
    is_.read(reinterpret_cast<char*>(&s), sizeof(s));
    return is_.good();
  }
  virtual bool Deserialize(int32_t& s) override {
    is_.read(reinterpret_cast<char*>(&s), sizeof(s));
    return is_.good();
  }
  virtual bool Deserialize(uint32_t& s) override {
    is_.read(reinterpret_cast<char*>(&s), sizeof(s));
    return is_.good();
  }
  virtual bool Deserialize(int64_t& s) override {
    is_.read(reinterpret_cast<char*>(&s), sizeof(s));
    return is_.good();
  }
  virtual bool Deserialize(uint64_t& s) override {
    is_.read(reinterpret_cast<char*>(&s), sizeof(s));
    return is_.good();
  }
  virtual bool Deserialize(double& s) override {
    is_.read(reinterpret_cast<char*>(&s), sizeof(s));
    return is_.good();
  }
  virtual bool Deserialize(float& s) override {
    is_.read(reinterpret_cast<char*>(&s), sizeof(s));
    return is_.good();
  }
  virtual bool Deserialize(std::string& s) override {
    size_t size;
    if (!Deserialize(size)) {return false;}
    s.resize(size);
    is_.read(&s[0], size);
    return is_.good();
  }
};


#endif // SERIALIZE_BASELINE_DESERIALIZER_H