#pragma once
#include "core/Datamodel/FrameData.h"
#include <QByteArray>
#include <memory>
#include <vector>

namespace fosd { 
namespace protocol {
// Receive complete frames; first matching parser owns validation. Extensions
// must construct immutable payloads and retain no writable aliases to them.
// Add an implementation and include it in the injected ParserFactory.
class IFrameParser {
public:
    virtual ~IFrameParser() = default;

    virtual QString protocolId() const = 0;     //获取协议包表示
    virtual bool matches(const QByteArray &packet) const = 0;   //匹配-快速判断数据包是否属于该协议
    virtual core::FrameDataPtr parse(const QByteArray &packet, QString *error = nullptr) const = 0;  //解析-将数据包解析为FrameData对象
};

using FrameParsers = std::vector<std::unique_ptr<IFrameParser>>;    //帧解析器集合

FrameParsers createFrameParsers();  //创建帧解析器集合

} 
} // namespace fosd::protocol
