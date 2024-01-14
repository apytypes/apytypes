#ifndef _APYFLOAT_CONTEXTMANAGER_H
#define _APYFLOAT_CONTEXTMANAGER_H
#include "apyfloat.h"
#include "apytypes_common.h"

class ContextManager {
public:
    virtual void enter_context() = 0;
    virtual void exit_context() = 0;
};

/*
    This allows the user to choose a rounding mode for all operations performed inside
   the runtime context. The rounding mode will be changed back to whatever it was before
   when the context ends.


    Python example using nested contexts:
    with RoundingContext(ROUND_MODE.TO_POSITIVE):
        # Operations now round towards negative infinity
        ...
        with RoundingContext(ROUND_MODE.TO_NEGATIVE):
            # Operations now round towards negative infinity
            ...
        # Operations now round towards positive infinity again
    # Rounding mode now reverted back to what was used before
*/
class RoundingContext : public ContextManager {
public:
    RoundingContext(const RoundingMode& new_mode);
    virtual void enter_context() override;
    virtual void exit_context() override;

private:
    RoundingMode new_mode, prev_mode;
};

// TODO: AccumulatorContext for matrix operations

#endif // _APYFLOAT_CONTEXTMANAGER_H
