#ifndef HW_SINGLETON_H
#define HW_SINGLETON_H

namespace hw {

template <typename T> class Singleton {
  public:
    static T& GetInstance() {
        static T instance;
        return instance;
    }

  protected:
    Singleton() = default;
    virtual ~Singleton() = default;
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;
};

} // namespace hw

#endif // HW_SINGLETON_H