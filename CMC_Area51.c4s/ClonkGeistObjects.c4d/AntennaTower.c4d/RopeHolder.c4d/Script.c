/*-- Seilhalterung --*/

#strict 2

local pAntenna, fDestroyed;
local pRopeHolder;

public func IsBulletTarget()		{return !fDestroyed;}
public func IsDestroyed()		{return fDestroyed;}
public func SetAntenna(object pObject)	{return pAntenna = pObject;}


public func SetFakeRopeHolder(object pObject) 
{ 
  //Objekt, welches von der Grafik her eine Seilhalterung sein soll
  pRopeHolder = pObject; 
  //Callback, dass diese Seilhalterung daran "befestigt" wurde
  pRopeHolder->~RopeHolderAttached(this);
}

/* Schaden */

public func Damage()
{
  if(!IsDestroyed() && GetDamage() > 100)
  {
    SetCategory(C4D_Object);
    pAntenna->RopeHolderDestroyed(this);

    //Callback an Fake, damit dieses zum Wrack werden kann
    if(pRopeHolder)
      pRopeHolder->~RopeHolderDestroyed(this);

    fDestroyed = true;

    var angle = Angle(GetX(), GetY(), GetX(pAntenna), GetY(pAntenna));
    SetXDir(+Sin(angle, 100)/2);
    SetYDir(-Cos(angle, 100)/2);

    Sound("RopeBreakOff*.ogg");
    Sound("RopeHit.ogg");
  }

  return true;
}