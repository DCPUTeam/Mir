///
/// @brief The implementation of the message that repositions objects.
/// @author James Rhodes
///

#define MIR_NETWORK_CONTROLLER_INTERNALS

#include <sstream>
#include <iostream>
#include <exception>
#include <boost/tokenizer.hpp>
#include <string>
#include "RepositionMessage.h"

namespace Network
{
    ///
    /// @brief Empty constructor used for deserialization.
    /// @internal
    ///
    RepositionMessage::RepositionMessage(Source* source)
        : ObjectMessage(source)
    {
    }

    ///
    /// @brief Constructs a repositioning message for the object with the given identifier.
    ///
    RepositionMessage::RepositionMessage(std::string identifier, bool xchanged, double x, bool ychanged, double y, bool zchanged, double z)
        : ObjectMessage(false)
    {
        this->Identifier = identifier;
        this->XChanged = xchanged;
        this->YChanged = ychanged;
        this->ZChanged = zchanged;
        this->X = x;
        this->Y = y;
        this->Z = z;
    }

    ///
    /// @brief Serializes the request message.
    /// @internal
    ///
    std::string RepositionMessage::Serialize()
    {
        std::stringstream ss;
        ss << this->Identifier;
        ss << ":";
        ss << this->XChanged;
        ss << ":";
        ss << this->YChanged;
        ss << ":";
        ss << this->ZChanged;
        ss << ":";
        ss << this->X;
        ss << ":";
        ss << this->Y;
        ss << ":";
        ss << this->Z;
        return ss.str();
    }

    ///
    /// @brief Deserializes a request message.
    /// @internal
    ///
    void RepositionMessage::Deserialize(std::string data)
    {
        typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
        boost::char_separator<char> sep(":");
        tokenizer tokens(data, sep);
        tokenizer::iterator i = tokens.begin();
        this->Identifier = *(i++);
        if (!(std::stringstream(*(i++)) >> this->XChanged))
            this->XChanged = false;
        if (!(std::stringstream(*(i++)) >> this->YChanged))
            this->YChanged = false;
        if (!(std::stringstream(*(i++)) >> this->ZChanged))
            this->ZChanged = false;
        if (!(std::stringstream(*(i++)) >> this->X))
            this->X = 0;
        if (!(std::stringstream(*(i++)) >> this->Y))
            this->Y = 0;
        if (!(std::stringstream(*(i++)) >> this->Z))
            this->Z = 0;
    }

    int RepositionMessage::GetHashCode()
    {
        int i = 0;
        for (size_t j = 0; j < std::string("RepositionMessage").length(); j++)
            i += i * "RepositionMessage"[i];
        for (size_t j = 0; j < this->Identifier.length(); j++)
            i += i * this->Identifier[i];
        return i;
    }
}
