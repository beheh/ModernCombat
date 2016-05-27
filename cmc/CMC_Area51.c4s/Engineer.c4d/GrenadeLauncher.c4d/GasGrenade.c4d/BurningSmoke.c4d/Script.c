/*-- Aerosol --*/

#strict 2

local iLifeTime;
local fBurning;

static const BGSE_FadeTime = 175;		//5 Sekunden


/* Initalisierung */

public func Initialize()
{
  fBurning = false;  iLifeTime = 35*20+Random(35*5);
  SetCon(5);
  SetAction("Be");

  //Raucheffekt
  AddEffect("Smoking", this, 25, 4, this);
}

/* Timecall */

public func Timer()
{
  //Bei Wasserkontakt verschwinden
  if(InLiquid() || FindObject2(Find_AtPoint(0, 0), Find_ID(SM4K), Find_Func("IsSmoking")))
  {
    if(fBurning)
      Sound("Pshshsh");
    RemoveObject();
  }

  //Laufzeit aufgebraucht: Verschwinden
  if(GetActTime() > iLifeTime)
    RemoveObject();

  //Rauchwolken vergrößern
  if(GetCon() < 50)
    DoCon(6);
  //Bewegung verlangsamen
  Damp(GetCon());
    SetYDir(GetYDir(0,1000)+GetGravityAccel4K(500),0,1000);

  //Lebewesen verbrennen falls entzündet
  if(fBurning)
  {
    for(var obj in FindObjects(Find_Distance(GetCon()/2,0,0), Find_NoContainer(), Find_OCF(OCF_Living | OCF_CrewMember)))
    {
      if(!GetEffect("BurningSmokeGrenade", obj))
        AddEffect("BurningSmokeGrenade", obj, 1, 1, obj, 0, GetOwner());
    }
  }
}

/* Raucheffekt */

public func FxSmokingTimer()
{
  var alpha = BoundBy((GetActTime()-(iLifeTime-BGSE_FadeTime)) * 255 / BGSE_FadeTime, 0, 255);
  CreateParticle("GunSmoke", 0, 0, 0, RandomX(-10, +10), GetCon()*10, RGBa(255, 255, 255, alpha));
}

public func FxBurningTimer()
{
  var alpha = BoundBy((GetActTime()-(iLifeTime-BGSE_FadeTime)) * 255 / BGSE_FadeTime, 0, 255);
  CreateParticle("Fire2", 0, 0, 0, RandomX(-10, +10), GetCon()*10, RGBa(255, 255, 255, alpha));
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

global func FxBurningSmokeGrenadeStart(object pTarget, int iEffectNumber, int iTemp, owner)
{
  //Zur Punkteberechnung
  EffectVar(1, pTarget, iEffectNumber) = owner;

  //KI-gesteuerte Objekte ignorieren
  if(GetController(pTarget) == NO_OWNER) return;
  if(GetPlayerType(GetController(pTarget)) == C4PT_Script) return;

  EffectVar(0, pTarget, iEffectNumber) = ScreenRGB(pTarget, RGBa(255, 150, 150, 200), 0, 0, false, SR4K_LayerSmoke);
  return;
}

global func FxBurningSmokeGrenadeTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  //Sichteffekt ermitteln
  var rgb = EffectVar(0, pTarget, iEffectNumber);
  if(!rgb) return 0;

  //Objekt noch im Rauch?
  var smoked = false;
  for(var smoke in FindObjects(pTarget->Find_AtPoint(), Find_ID(BGSE), Find_Func("IsSmoking")))
  {
    if(GetCon(smoke)/2 > Distance(GetX(smoke),GetY(smoke),GetX(pTarget),GetY(pTarget)))
    {
      smoked = true;
      break;
    }
  }

  //Bildschirmeffekt verdunkeln oder auflösen
  if(smoked)
  {
  	BurnObjectWithSmoke(pTarget);
    rgb->DoAlpha(+10, 0, 200);
  }
  else
  {
    rgb->DoAlpha(-10, 0, 200);
    if(rgb->GetAlpha() >= 200)
      return -1;
  }
}

global func FxBurningSmokeGrenadeStop(object pTarget, int iEffectNumber, int iReason, bool fTemp)
{
  if(EffectVar(0,pTarget,iEffectNumber))
    RemoveObject(EffectVar(0,pTarget,iEffectNumber));
}

public func SetAlpha(int iValue)
{
  var a = BoundBy(iValue,0,200);
  var r,g,b;
  SplitRGBaValue(iValue,r,g,b);
  SetClrModulation(RGBa(r,g,b,a));
}

public func DoAlpha(int iValue, int iMin, int iMax)
{
  if(!iMax) iMax = 200;
  var r,g,b,a;
  SplitRGBaValue(iValue,r,g,b,a);
  a = BoundBy(a-iValue,Max(iMin,0),Min(iMax,200));
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

global func BurnObjectWithSmoke(pObj)
{
  //Lebewesen mit Effekt belegen
  if(GetOCF(pObj) & OCF_Living)
    AddEffect("Burning", pObj, 50, 20, this, GetID());

  //Schaden verursachen
  DoDmg(6, DMG_Fire, pObj, 1);
  AddFireEffect(pObj,50,FIRE_Red,1);

  //Anzündbares anzünden, aber nicht Lebewesen
  if(pObj)
    if(GetOCF(pObj) & OCF_Inflammable && !(GetOCF(pObj) & OCF_Living))
      Incinerate(pObj, GetController()+1);

  //Effekte
  if(!Random(20))
    Sound("Crackle.ogg",0,0,RandomX(20,40));

  return true;
}

/* Entzündung */

public func Incineration()
{
  //Raucheffekt gegen Brandeffekt tauschen
  if(!fBurning)
  {
    RemoveEffect("Smoking", this);
    AddEffect("Burning", this, 25, 4, this);
    fBurning = true;

    //Effekte
    Sound("GasInflame*.ogg");
    Sound("GasFire.ogg", false, this, 50, 0, 1);
  }

  Extinguish();
}