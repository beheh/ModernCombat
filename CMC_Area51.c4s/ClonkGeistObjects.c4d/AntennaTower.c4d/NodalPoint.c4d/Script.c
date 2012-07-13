/*-- Knotenpunkt --*/

#strict 2

local pAntenna, fDestroyed;
local pRopeHolder;
local iLastDmgPlr;

public func IsBulletTarget()		{return !fDestroyed;}
public func IsDestroyed()		{return fDestroyed;}
public func SetAntenna(object pObject)	{return pAntenna = pObject;}


public func SetRopeHolder(object pObject) 
{ 
  //Objekt, welches von der Grafik her eine Seilhalterung sein soll
  pRopeHolder = pObject; 
  //Callback, dass diese Seilhalterung daran "befestigt" wurde
  pRopeHolder->~NodeAttached(this);
  //Shape an Größe der Seilhalterung anpassen
  var id = GetID(pRopeHolder);
  var wdt = GetDefWidth(id), hgt = GetDefHeight(id);
  var offX = GetDefOffset(id, 0), offY = GetDefOffset(id, 1);

  var pX = GetX(), pY = GetY();
  var x = GetX(pRopeHolder) - offX - wdt/2, y = GetY(pRopeHolder) - offY - wdt/2;
  SetPosition(x, y); //Mittig der Halterung platzieren
  SetShape(-wdt/2, -hgt/2, wdt, hgt); //Shape setzen
  SetVertex(0, 0, AbsX(pX));
  SetVertex(0, 1, AbsY(pY));

  return true;
}

/* Schaden */

public func Damage()
{
  if(!IsDestroyed() && GetDamage() > 100)
  {
    SetCategory(C4D_Object);
    SetPosition(GetX()+GetVertex(0), GetY()+GetVertex(0, true));

    SetShape(-3, -3, 6, 6);
    SetVertex();
    SetVertex(0, 1, 1);
    var nodes = pAntenna->NodeDestroyed(this);
    SetOwner(iLastDmgPlr);

    //Callback an Halterung, damit dieses zum Wrack werden kann
    if(pRopeHolder)
      pRopeHolder->~NodeDestroyed(this);

    fDestroyed = true;

    var pOtherNode = nodes[!GetIndexOf(this, nodes)];
    var angle = Angle(GetX(), GetY(), GetX(pOtherNode), GetY(pOtherNode));
    SetXDir(+Sin(angle, 100)/2);
    SetYDir(-Cos(angle, 100)/2);

    Sound("RopeBreakOff*.ogg");
    Sound("RopeHit.ogg");
  }

  return true;
}

public func OnHit(int damage, int type, object pFrom)
{
  iLastDmgPlr = GetController(pFrom);
  return true;
}