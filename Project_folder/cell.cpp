#include "cell.h"

Cell::Cell() : unit_(nullptr) {}

bool Cell::IsOccupied() const {
    return unit_ != nullptr;
}

Unit* Cell::GetUnit() const {
    return unit_;
}

void Cell::SetUnit(Unit* unit) {
    unit_ = unit;
}

Unit* Cell::ReleaseUnit() {
    Unit* temp = unit_;
    unit_ = nullptr;
    return temp;
}