/*-- Schild --*/

#strict 2

local mode;


/* Intialisierung */

func Initialize()
{
  SetAction("Exist");
}

/* Einstellungen */

func SetMode(int iMode)
{
  if(iMode == true)
  {
    SetGraphics("Metal");
    mode = 1;
  }
  else
  {
    SetGraphics("");
    mode = 0;
  }
}

/* Zerstörung */

func Damage()
{
  if(GetDamage() > 80)
  {
    //Effekte
    if(mode)
    {
      CastParticles("MetalSplinter", 8, 50, 0,0, 141);
    }
    else
    {
      CastParticles("WoodSplinter", 8, 50, 0,0, 141);
    }
    CastSmoke("Smoke3",12,10,0,0,100,200,RGBa(255,255,255,100),RGBa(255,255,255,130));
    Sound("CrateCrack.ogg");

    RemoveObject();
  }
}