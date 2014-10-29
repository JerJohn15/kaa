/*
 * Copyright 2014 CyberVision, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef AVROBYTEARRAYCONVERTER_HPP_
#define AVROBYTEARRAYCONVERTER_HPP_

#include <string>
#include <memory>
#include <sstream>

#include <avro/Compiler.hh>
#include <avro/Specific.hh>
#include <avro/Stream.hh>
#include <avro/Encoder.hh>
#include <avro/Decoder.hh>

#include <boost/cstdint.hpp>

#include "kaa/common/EndpointObjectHash.hpp"
#include "kaa/common/exception/KaaException.hpp"

namespace kaa {

/**
 * Used to convert predefined avro objects to/from bytes.
 * NOT Thread safe.
 * @param <T> predefined avro object.
 */
template<typename T>
class AvroByteArrayConverter {
public:
    /**
     * Instantiates a new avro byte array converter based on <T>.
     */
    AvroByteArrayConverter();


    /*
     * Copy operator

     */

    /**
     * Creates avro object from byte array
     * Throws \ref KaaException when invalid data was passed (zero-sized or null buffer)
     * @param data the data
     * @param dataSize size of data
     * @return the result of conversion
     */
    T fromByteArray(const boost::uint8_t* data, const boost::uint32_t& dataSize);

    /**
     * Creates avro object from byte array
     * Throws \ref KaaException when invalid data was passed (zero-sized or null buffer)
     * @param data the data
     * @param dataSize size of data
     * @param the result of conversion
     */
    void fromByteArray(const boost::uint8_t* data, const boost::uint32_t& dataSize, T& datum);

    /**
     * Converts object to byte array
     * @param datum the encoding avro object
     * @return SharedDataBuffer result of a conversion
     */
    SharedDataBuffer toByteArray(const T& datum);

    /**
     * Converts object to stream
     * @param datum the encoding avro object
     * @param stream the output stream into which encoded data will be put
     */
    void toByteArray(const T& datum, std::ostream& stream);

    /**
     * Used for debug purpose
     */
    void switchToJson(const avro::ValidSchema &schema) {
        encoder_ = avro::jsonEncoder(schema);
        decoder_ = avro::jsonDecoder(schema);
    }

    void switchToBinary() {
        encoder_ = avro::binaryEncoder();
        decoder_ = avro::binaryDecoder();
    }

private:
    avro::EncoderPtr   encoder_;
    avro::DecoderPtr   decoder_;
};

template<typename T>
AvroByteArrayConverter<T>::AvroByteArrayConverter()
{
    switchToBinary();
}

template<typename T>
T AvroByteArrayConverter<T>::fromByteArray(const boost::uint8_t* data, const boost::uint32_t& dataSize)
{
    if (!data || dataSize == 0) {
        throw KaaException("null data to decode");
    }

    T datum;
    std::unique_ptr<avro::InputStream> in = avro::memoryInputStream(data, dataSize);

    decoder_->init(*in);
    avro::decode(*decoder_, datum);

    return datum;
}

template<typename T>
void AvroByteArrayConverter<T>::fromByteArray(const boost::uint8_t* data, const boost::uint32_t& dataSize, T& datum)
{
    if (!data || dataSize == 0) {
        throw KaaException("null data to decode");
    }

    std::unique_ptr<avro::InputStream> in = avro::memoryInputStream(data, dataSize);

    decoder_->init(*in);
    avro::decode(*decoder_, datum);
}

template<typename T>
SharedDataBuffer AvroByteArrayConverter<T>::toByteArray(const T& datum)
{
    std::ostringstream ostream;
    std::unique_ptr<avro::OutputStream> out = avro::ostreamOutputStream(ostream);

    encoder_->init(*out);
    avro::encode(*encoder_, datum);
    encoder_->flush();

    SharedDataBuffer buffer;
    const std::string& encodedData = ostream.str();
    const size_t encodedDataSize = encodedData.size();

    buffer.second = encodedDataSize;
    buffer.first.reset(new uint8_t[encodedDataSize]);
    memcpy(buffer.first.get(), encodedData.data(), encodedDataSize);

    return buffer;
}

template<typename T>
void AvroByteArrayConverter<T>::toByteArray(const T& datum, std::ostream& stream)
{
    std::unique_ptr<avro::OutputStream> out = avro::ostreamOutputStream(stream);

    encoder_->init(*out);
    avro::encode(*encoder_, datum);
    encoder_->flush();
}

} /* namespace kaa */

#endif /* AVROBYTEARRAYCONVERTER_HPP_ */