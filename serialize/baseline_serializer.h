#ifndef SERIALIZE_BASELINE_SERIALIZER_H
#define SERIALIZE_BASELINE_SERIALIZER_H

#include "serialize/serializer.h"

// Baseline实现
class BaselineSerializer : public Serializer {
 public:
  BaselineSerializer() = delete;
  BaselineSerializer(std::ostream& os): Serializer(os) {}
  // virtual 不支持模板, 需要全部列出
  virtual void Serialize(char s) override {
    os_.write(reinterpret_cast<const char*>(&s), sizeof(s));
  }
  virtual void Serialize(int16_t s) override {
    os_.write(reinterpret_cast<const char*>(&s), sizeof(s));
  }
  virtual void Serialize(uint16_t s) override {
    os_.write(reinterpret_cast<const char*>(&s), sizeof(s));
  }
  virtual void Serialize(int32_t s) override {
    os_.write(reinterpret_cast<const char*>(&s), sizeof(s));
  }
  virtual void Serialize(uint32_t s) override {
    os_.write(reinterpret_cast<const char*>(&s), sizeof(s));
  }
  virtual void Serialize(int64_t s) override {
    os_.write(reinterpret_cast<const char*>(&s), sizeof(s));
  }
  virtual void Serialize(uint64_t s) override {
    os_.write(reinterpret_cast<const char*>(&s), sizeof(s));
  }
  virtual void Serialize(double s) override {
    os_.write(reinterpret_cast<const char*>(&s), sizeof(s));
  }
  virtual void Serialize(float s) override {
    os_.write(reinterpret_cast<const char*>(&s), sizeof(s));
  }
  virtual void Serialize(const std::string& s) override {
    size_t size = s.size();
    Serialize(size);
    os_.write(s.data(), size);
  };
};


#endif // SERIALIZE_BASELINE_SERIALIZER_H