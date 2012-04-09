/*-- Konsole --*/

#strict 2

#appendto CONS

public func IsFakeRepairable() { return (target && target->~IsRepairable() && GetDamage(target)); }

public func GetRealRepairableObject() { return target; } 
