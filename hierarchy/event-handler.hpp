#pragma ince

#include "linear-hierarchy.hpp"

enum class EEventType {
    EtClear
};

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename TObjectType, typename TBase = TTypeList<>>
class TEventHandler : public TBase {
public:
    virtual void HandleEvent(TObjectType&, EEventType) = 0;
    virtual ~TEventHandler() = default;
};
