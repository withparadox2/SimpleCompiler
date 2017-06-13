//
// Created by ruancongyong on 2017/6/13.
//

#ifndef SIMPLECOMPILER_STACKFRAME_H
#define SIMPLECOMPILER_STACKFRAME_H

#include <memory>
#include <vector>
#include "../code/types.h"
#define SAME_FRAME_SIZE 64
#define SAME_LOCALS_1_STACK_ITEM_EXTENDED 247
#define SAME_FRAME_EXTENDED 251
#define FULL_FRAME 255
#define MAX_LOCAL_LENGTH_DIFF 4

class ClassWriter;
class StackMapTableFrame {
public:
    typedef std::shared_ptr<StackMapTableFrame> Ptr;
    typedef std::vector<Ptr> List;

    virtual int getFrameType();

    virtual void write(ClassWriter& writer);

    void writeStackMapType(ClassWriter& writer, TypePtr& t);
};

class SameFrame : public StackMapTableFrame {
private:
    int offsetDelta;
public:
    typedef std::shared_ptr<SameFrame> Ptr;
    SameFrame(int offsetDelta);

    int getFrameType() override;

    void write(ClassWriter& writer) override;
};


class ChopFrame : public StackMapTableFrame {
private:
    int frameType;
    int offsetDelta;
public:
    typedef std::shared_ptr<ChopFrame> Ptr;

    ChopFrame(int frameType, int offsetDelta);

    int getFrameType() override;

    void write(ClassWriter& writer) override;
};

class AppendFrame : public StackMapTableFrame {
private:
    int frameType;
    int offsetDelta;
    TypeList locals;
public:
    typedef std::shared_ptr<AppendFrame> Ptr;

    AppendFrame(int frameType, int offsetDelta, TypeList locals);

    int getFrameType() override;

    void write(ClassWriter& writer) override;
};

#endif //SIMPLECOMPILER_STACKFRAME_H
