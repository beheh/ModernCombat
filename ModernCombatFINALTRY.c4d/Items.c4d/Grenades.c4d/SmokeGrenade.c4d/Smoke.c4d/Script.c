/*-- Rauch --*/

#strict 2

local iLifeTime;
local fSmoking;
static const SM4K_FadeTime = 105;	//3 Sekunden

public func IsSmoking()		{return fSmoking;}


/* Initialisierung */

public func Initialize()
{
  fSmoking = true;

  //Lebenszeit und Gr��e setzen
  iLifeTime = 35*25+Random(35*2);
  SetCon(5);
  SetAction("Be");

  //Raucheffekt
  AddEffect("Smoking", this, 25, 5, this);
}

/* Timecall */

public func Timer()
{
  //In Wasser aufl�sen
  if(InLiquid())
    RemoveObject();

  //Zuende?
  if(GetActTime() > iLifeTime)
    RemoveObject();
  //Nicht mehr vernebeln?
  if (GetActTime() > iLifeTime-SM4K_FadeTime/2)
    fSmoking = false;
  if(GetActTime() > iLifeTime-SM4K_FadeTime) //Rauchen einstellen
    return;

  //Rauchwolke auf bestimmte Gr��e anwachsen lassen
  if(GetCon() < 50)
    DoCon(6);

  //Gr��enabh�ngig verlangsamen
  Damp(GetCon());
    SetYDir(GetYDir(0,1000)+GetGravityAccel4K(500),0,1000);

  //Zu blendende Objekte suchen
  for(var obj in FindObjects(Find_Distance(GetCon()/2,0,0), Find_NoContainer(), Find_OCF(OCF_Living | OCF_CrewMember)))
  {
    //Raucheffekt hinzuf�gen falls nicht vorhanden
    if(!GetEffect("SmokeGrenade", obj))
      AddEffect("SmokeGrenade", obj, 1, 2, this);
  }
}

public func FxSmokingTimer()
{
  var alpha = BoundBy((GetActTime()-(iLifeTime-SM4K_FadeTime)) * 255 / SM4K_FadeTime, 0, 255);
  CreateParticle("SmokeGrenadeSmoke", 0, 0, 0, RandomX(-10, +10), GetCon()*10, RGBa(255, 255, 255, alpha));
}

/* Kontakt */

protected func ContactTop()
{
  Spread();
}

protected func ContactBottom()
{
  Spread();
}

protected func ContactLeft()
{
  Spread();
}

protected func ContactRight()
{
  Spread();
}

func Spread()
{
  var contact = GetContact(this,-1);
  if(!contact) return;
  if(contact & CNAT_Bottom)
    SetYDir(GetYDir(0,100)-30,0,100);
  if(contact & CNAT_Top)
    SetYDir(GetYDir(0,100)+30,0,100);
  if(contact & CNAT_Right)
    SetXDir(GetXDir(0,100)-30,0,100);
  if(contact & CNAT_Left)
    SetXDir(GetXDir(0,100)+30,0,100);
}

/* Raucheffekt */

global func FxSmokeGrenadeStart(object pTarget, int iEffectNumber, int iTemp)
{
  //Besitzerlose Ziele nicht blenden
  if(GetController(pTarget) == NO_OWNER) return;
  if(GetPlayerType(GetController(pTarget)) == C4PT_Script) return;
  EffectVar(0, pTarget, iEffectNumber) = ScreenRGB(pTarget, RGBa(150, 150, 150, 255), 0, 0, false, SR4K_LayerSmoke);
  return;
}

global func FxSmokeGrenadeTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  //Keine Blendung �brig? Verschwinden
  var rgb = EffectVar(0, pTarget, iEffectNumber);
  if(!rgb) return -1;

  //Noch im Rauch?
  var smoked = false, friend;
  for(var smoke in FindObjects(pTarget->Find_AtPoint(), Find_ID(SM4K), Find_Func("IsSmoking"),Find_Hostile(GetOwner(pTarget))))
  {
    if(GetCon(smoke)/2 > Distance(GetX(smoke),GetY(smoke),GetX(pTarget),GetY(pTarget)))
    {   
      smoked = true;
      break;
    }
  }
  if(!smoked)
  for(var smoke in FindObjects(pTarget->Find_AtPoint(), Find_ID(SM4K), Find_Func("IsSmoking"),))
  {
    if(GetCon(smoke)/2 > Distance(GetX(smoke),GetY(smoke),GetX(pTarget),GetY(pTarget)))
    {
      friend = 1;
      smoked = true;
      break;
    }
  }

  //Intensit�t je nach Freund/Feind
  if(smoked)
    rgb->DoAlpha(+20, friend*128, 254);
  else
    rgb->DoAlpha(-20, 0, 254);
}

global func FxSmokeGrenadeStop(object pTarget, int iEffectNumber, int iReason, bool fTemp)
{
  if(EffectVar(0,pTarget,iEffectNumber))
    RemoveObject(EffectVar(0,pTarget,iEffectNumber));
}

/* Geschwindigkeitsd�mpfung */

public func Damp(int iStrength)
{
  iStrength = 1000 - iStrength;
  SetXDir(Mul4K(iStrength, GetXDir(this, 1000), 1000), this, 1000);
  SetYDir(Mul4K(iStrength, GetYDir(this, 1000), 1000), this, 1000);
}