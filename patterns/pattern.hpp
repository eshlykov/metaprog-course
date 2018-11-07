#pragma once

enum class EPatternKind {
    PkProxy,
    PkObserver,
    PkMediator
};

template <EPatternKind patternKind>
class TPattern {
};

using TProxy = TPattern<EPatternKind::PkProxy>;
using TObserver = TPattern<EPatternKind::PkObserver>;
using TMediator = TPattern<EPatternKind::PkMediator>;
