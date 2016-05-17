/*-- Knotenpunkt --*/

#strict 2

local pAntenna, fDestroyed;
local pRopeHolder;
local iLastDmgPlr;

public func IsDestroyed()		{return fDestroyed;}
public func SetAntenna(object pObject)	{return pAntenna = pObject;}


/* Verbinden */

public func SetRopeHolder(object pObject) 
{ 
  //Objekt für Seilhalterungsimitierung
  pRopeHolder = pObject;
  //Knotenpunkt daran befestigen
  pRopeHolder->~NodeAttached(this);
  //Shape an Größe der Seilhalterung anpassen
  var id = GetID(pRopeHolder);
  var wdt = GetDefWidth(id), hgt = GetDefHeight(id);
  var offX = GetDefOffset(id, 0), offY = GetDefOffset(id, 1);
  var pX = GetX(), pY = GetY();
  var x = GetX(pRopeHolder) - offX - wdt/2, y = GetY(pRopeHolder) - offY - wdt/2;
  //Mittig an Halterung plazieren
  SetPosition(x, y);
  //Shape setzen
  SetShape(-wdt/2, -hgt/2, wdt, hgt);
  SetVertex(0, 0, AbsX(pX));
  SetVertex(0, 1, AbsY(pY));

  return true;
}

/* Schaden */

public func Damage()
{
  if(!IsDestroyed() && GetDamage() > 30)
  {
    SetCategory(C4D_Object);
    SetPosition(GetX()+GetVertex(0), GetY()+GetVertex(0, true));

    //Shape aktualisieren
    SetShape(-3, -3, 6, 6);
    SetVertex();
    SetVertex(0, 1, 1);
    //Mast informieren
    var nodes = pAntenna->NodeDestroyed(this);
    //Killer setzen
    SetOwner(iLastDmgPlr);
    //Zugehörige Seilhalterung zerstören sofern vorhanden
    if(pRopeHolder)
      pRopeHolder->~NodeDestroyed(this);

    fDestroyed = true;

    //In Bewegung setzen
    var pOtherNode = nodes[!GetIndexOf(this, nodes)];
    var angle = Angle(GetX(), GetY(), GetX(pOtherNode), GetY(pOtherNode));
    SetXDir(+Sin(angle, 100)/2);
    SetYDir(-Cos(angle, 100)/2);

    //Effekte
    FadeOut();
    Sound("WreckHit*.ogg");
    Sound("RopeHit.ogg");
  }

  return true;
}

public func OnHit(int damage, int type, object pFrom)
{
  iLastDmgPlr = GetController(pFrom);

  Sound("RopeHit.ogg",0,0,50);

  return true;
}

/* Aufschlag */

public func Hit3()
{
  Sound("RopeHit.ogg",0,0,50);
}