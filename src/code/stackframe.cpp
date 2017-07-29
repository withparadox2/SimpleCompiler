//
// Created by ruancongyong on 2017/6/13.
//

#include "stackframe.h"
#include "type.h"
#include "util/error.h"
#include "jvm/ClassWriter.h"

void StackMapTableFrame::write(ClassWriter& writer) {
    int frameType = getFrameType();
    writer.append1(writer.databuf, frameType);
}

void StackMapTableFrame::writeStackMapType(ClassWriter& writer, TypePtr& t) {
    if (t.get() == nullptr) {
        writer.append1(writer.databuf, 0);
    } else {
        int typeByte = 0;
        switch (t->tag) {
            case TypeTags::BYTE:
            case TypeTags::CHAR:
            case TypeTags::SHORT:
            case TypeTags::INT:
            case TypeTags::BOOLEAN:
                typeByte = 1;
                break;
            case TypeTags::FLOAT:
                typeByte = 2;
                break;

            case TypeTags::DOUBLE:
                typeByte = 3;
                break;

            case TypeTags::LONG:
                typeByte = 4;
                break;

            case TypeTags::BOT:
                typeByte = 5;
                break;
            default:
                error("can not handle type:" + std::to_string(t->tag) + " in writeStackMapType");
                break;
        }
        writer.append1(writer.databuf, typeByte);
    }

}

int StackMapTableFrame::getFrameType() {
    return 0;
}

SameFrame::SameFrame(int offsetDelta) : offsetDelta(offsetDelta) {
}

int SameFrame::getFrameType() {
    return offsetDelta < SAME_FRAME_SIZE ? offsetDelta : SAME_FRAME_EXTENDED;
}

void SameFrame::write(ClassWriter& writer) {
    StackMapTableFrame::write(writer);
    if (getFrameType() == SAME_FRAME_EXTENDED) {
        writer.append2(writer.databuf, offsetDelta);
    }
}

ChopFrame::ChopFrame(int frameType, int offsetDelta)
        : frameType(frameType), offsetDelta(offsetDelta) {

}

int ChopFrame::getFrameType() {
    return frameType;
}

void ChopFrame::write(ClassWriter& writer) {
    StackMapTableFrame::write(writer);
    writer.append2(writer.databuf, offsetDelta);
}

AppendFrame::AppendFrame(int frameType, int offsetDelta, TypeList locals)
        : frameType(frameType), offsetDelta(offsetDelta), locals(locals) {

}

int AppendFrame::getFrameType() {
    return frameType;
}

void AppendFrame::write(ClassWriter& writer) {
    StackMapTableFrame::write(writer);
    writer.append2(writer.databuf, offsetDelta);
    for (auto iter = locals.begin(); iter != locals.end(); iter++) {
        writeStackMapType(writer, *iter);
    }
}
