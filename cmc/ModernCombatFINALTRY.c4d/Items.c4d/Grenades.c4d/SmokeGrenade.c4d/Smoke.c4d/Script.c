/*-- Rauch --*/

#strict 2

local iLifeTime;
local fSmoking;

static const SM4K_FadeTime = 175;		//5 Sekunden

public func IsSmoking()	{return fSmoking;}


/* Initalisierung */

public func Initialize()
{
  fSmoking = true;
  iLifeTime = 35*20+Random(35*5);
  SetCon(5);
  SetAction("Be");

  //Raucheffekt
  AddEffect("Smoking", this, 25, 4, this);
}

/* Peilsenderentfernung */

private func DestroyTracers()
{
  for(var obj in FindObjects(Find_Distance(40),
  				Find_Category(C4D_Living | C4D_Structure | C4D_Vehicle),
  				Find_Allied(GetOwner())))
    if(GetEffect("TracerDart",obj))
      RemoveEffect("TracerDart",obj);
}

/* Timecall */

public func Timer()
{
  //Bei Wasserkontakt verschwinden
  if(InLiquid())
    RemoveObject();
    
  //Peilsendereffekte entfernen
  if(GetActTime() >= 35 && GetActTime() <= 70)
  	DestroyTracers();

  //Laufzeit aufgebraucht: Verschwinden
  if(GetActTime() > iLifeTime)
    RemoveObject();
  //Nicht mehr blenden?
  if(GetActTime() > iLifeTime-SM4K_FadeTime/3)
    fSmoking = false;
  //Rauchen einstellen
  if(GetActTime() > iLifeTime-SM4K_FadeTime)
    return;

  //Rauchwolken vergrößern
  if(GetCon() < 50)
    DoCon(6);
  //Bewegung verlangsamen
  Damp(GetCon());
    SetYDir(GetYDir(0,1000)+GetGravityAccel4K(500),0,1000);

  //Lebewesen blenden
  for(var obj in FindObjects(Find_Distance(GetCon()/2,0,0), Find_NoContainer(), Find_OCF(OCF_Living | OCF_CrewMember)))
  {
    if(!GetEffect("SmokeGrenade", obj))
      AddEffect("SmokeGrenade", obj, 1, 1, obj, 0, GetOwner());
  }
}

/* Raucheffekt */

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

/* Blendeffekt */

global func FxSmokeGrenadeStart(object pTarget, int iEffectNumber, int iTemp, owner)
{
  //Zur Punkteberechnung
  EffectVar(1, pTarget, iEffectNumber) = owner;

  //KI-gesteuerte Objekte ignorieren
  if(GetController(pTarget) == NO_OWNER) return;
  if(GetPlayerType(GetController(pTarget)) == C4PT_Script) return;

  EffectVar(0, pTarget, iEffectNumber) = ScreenRGB(pTarget, RGBa(150, 150, 150, 254), 0, 0, false, SR4K_LayerSmoke);
  return;
}

global func FxSmokeGrenadeTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  //Sichteffekt ermitteln
  var rgb = EffectVar(0, pTarget, iEffectNumber);
  if(!rgb) return 0;

  //Objekt noch im Rauch?
  var smoked = false;
  if(!Contained() || GetID(Contained()) == FKDT)
  {
    for(var smoke in FindObjects(pTarget->Find_AtPoint(), Find_ID(SM4K), Find_Func("IsSmoking")))
    {
      if(GetCon(smoke)/2 > Distance(GetX(smoke),GetY(smoke),GetX(pTarget),GetY(pTarget)))
      {
        smoked = true;
        break;
      }
    }
  }

  //Bildschirmeffekt verdunkeln oder auflösen
  if(smoked)
    rgb->DoAlpha(+10, 0, 254);
  else
  {
    rgb->DoAlpha(-10, 0, 254);
    if(rgb->GetAlpha() >= 254)
      return -1;
  }
}

global func FxSmokeGrenadeStop(object pTarget, int iEffectNumber, int iReason, bool fTemp)
{
  if(EffectVar(0,pTarget,iEffectNumber))
    RemoveObject(EffectVar(0,pTarget,iEffectNumber));
}

public func SetAlpha(int iValue)
{
  var a = BoundBy(iValue,0,255);
  var r,g,b;
  SplitRGBaValue(iValue,r,g,b);
  SetClrModulation(RGBa(r,g,b,a));
}

public func DoAlpha(int iValue, int iMin, int iMax)
{
  if(!iMax) iMax = 255;
  var r,g,b,a;
  SplitRGBaValue(iValue,r,g,b,a);
  a = BoundBy(a-iValue,Max(iMin,0),Min(iMax,255));
  SetClrModulation(RGBa(r,g,b,a));
}

public func GetAlpha()
{
  var a;
  SplitRGBaValue(GetClrModulation(),0,0,0,a);
  return a;
}

/* Geschwindigkeitsdämpfung */

public func Damp(int iStrength)
{
  iStrength = 1000 - iStrength;
  SetXDir(Mul4K(iStrength, GetXDir(this, 1000), 1000), this, 1000);
  SetYDir(Mul4K(iStrength, GetYDir(this, 1000), 1000), this, 1000);
}