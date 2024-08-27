#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H
#include <exception>

class DuplicateBindingException : public std::exception
{
private:
    QString error_msg;
public:
    virtual const char * what() noexcept override;
};

#endif // EXCEPTIONS_H
