/*-- Leitkegel --*/

#strict 2

local pLight;


/* Initialisierung */

public func Initialize()
{
  //Standardfarbe: Rot
  SetColorDw(HSL(250, 250, 140, 50));
}

/* Einstellungen */

func Light()
{
  if(pLight)
  {
    if(pLight) RemoveObject(pLight);
  }
  else
  {
    pLight = AddLight(600,RGBa(155,255,155,70),this());
    pLight -> TurnOn();
  }
}

/* Schaden */

func Damage(int iChange, int iPlr)
{
  if(GetDamage() > 60)
  {
    CastParticles("Paper", RandomX(4, 6), 60, 0, 0, 40, 35, GetColorDw());
    Sound("WoodHit*");
    RemoveObject(0, 1);
  }

  //Umherfliegen
  SetSpeed(RandomX(-45, 45), RandomX(-65, -55));
  SetRDir(GetXDir()*2);
}

/* Aufschlag */

protected func Hit()
{
  Sound("WoodHit*");
  return 1;
}