/*-- Sentry Gun --*/

#strict
#appendto SEGU

public func EMPShock()
{
    if(Damaged) return(0);
    Damaged = 1;
    AddEffect("EMP_Damaged", this(), 180, 50, this(), GetID() );
    Schedule("CastObjects(SPRK, 1, 15, RandomX(-10, 10), RandomX(-15, 15));", 3, 15, this());
    SetClrModulation(RGB(100,100,100));
    return(1);
}