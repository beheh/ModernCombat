/*-- Schild --*/

#strict 2

local mode;


/* Intialisierung */

func Initialize()
{
  //Standardwerte setzen
  SetAction("Exist");
  Set("Danger");
}

/* Blinken */

func Blink()
{
  AddLightFlash(100,0,0,RGB(255,0,0));
}

/* Einstellungen */

func Set(string szIcon)
{
  //Grafik aufdrucken
  if(mode)
    SetGraphics("Metal",0,GetID(this),2,GFXOV_MODE_Action, szIcon);
  else
    SetGraphics(0,0,GetID(this),2,GFXOV_MODE_Action, szIcon);
}

func SetMode(int iMode)
{
  if(iMode == true)
  {
    mode = 1;
    SetGraphics("Metal");
    SetAction("Exist2");
    Set("Danger");
  }
  else
  {
    mode = 0;
    SetGraphics("");
    SetAction("Exist");
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
    Sound("CrateDestruct*.ogg");

    //Verschwinden
    RemoveObject();
  }
}