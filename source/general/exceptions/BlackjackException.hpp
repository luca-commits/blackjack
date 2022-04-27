#include <string>

class BlackjackException : public std::exception {
private:
    std::string _msg;
public:
    explicit BlackjackException(const std::string& message) : _msg(message) { };

    const char* what() const noexcept override {
        return _msg.c_str();
    }
};

#endif
