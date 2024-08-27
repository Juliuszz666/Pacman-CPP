#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H
#include <exception>
#include <QString>
#include <QException>
/* Do wyjebania */
class DuplicateBindingException : public std::exception
{
private:
    std::string error_msg;
public:
    explicit DuplicateBindingException(const QString& message)
        : error_msg(message.toUtf8()) {}

    const char* what() const noexcept override
    {
        return error_msg.c_str();
    }
};

#endif // EXCEPTIONS_H
