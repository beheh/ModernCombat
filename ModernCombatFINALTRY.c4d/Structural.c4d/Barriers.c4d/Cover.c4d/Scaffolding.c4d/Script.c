/*-- Gerüst --*/

#strict 2

local iStatus, damaged;


/* Aussehen festlegen */

func Set(int i)
{
  iStatus = i;
  SetGraphics(Format("%d", i));

  if(iStatus > 3)
    damaged = true;
  else
    damaged = false;
}

/* Zerstörung */

func Damage(int iChange)
{
  if(damaged) return ;
  if(GetDamage() < 80) return;
  Destruct();
}

func Destruct()
{
  if(damaged) return ;
  damaged = true;

  if(iStatus < 2)
  {
    //Effekte
    CastParticles("WoodSplinter",4,100,-20,-25,100,150);
    CastParticles("WoodSplinter",4,100,20,-25,100,150);
    CastSmoke("Smoke3",3,20,0,-25,100,200,RGBa(255,255,255,100),RGBa(255,255,255,130));
    Sound("WoodCrack*.ogg");

    SetGraphics("5");
  }
  else
  if(iStatus < 3)
  {
    //Effekte
    CastParticles("WoodSplinter",4,100,-20,-25,100,150);
    CastParticles("WoodSplinter",4,100,20,-25,100,150);
    CastSmoke("Smoke3",3,20,0,-25,100,200,RGBa(255,255,255,100),RGBa(255,255,255,130));
    Sound("WoodCrack*.ogg");

    SetGraphics("5");
  }
  else
  {
    //Effekte
    CastParticles("WoodSplinter",4,100,-25,-5,100,150);
    CastParticles("WoodSplinter",4,100,20,-30,100,150);
    CastSmoke("Smoke3",3,20,0,-25,100,200,RGBa(255,255,255,100),RGBa(255,255,255,130));
    Sound("WoodCrack*.ogg");

    SetGraphics("5");
  }
}

/* Schaden */

public func OnDmg(int iDmg, int iType)
{
  if(iType == DMG_Bio)		return 100;	//Säure und biologische Schadstoffe
}

/* Serialisierung */

public func Serialize(array& extra)
{
if(iStatus > 1)
    extra[GetLength(extra)] = Format("Set(%d)",iStatus);
}