/*-- Rauch 2.0 --*/
#strict 2

local iLifeTime;
static const SM4K_FadeTime = 35*5;

public func Initialize()
{
	iLifeTime = 35*20+Random(35*5);
	SetAction("Be");
	SetCon(5);
	AddEffect("Smoking", this, 25, 2, this);
}

public func Timer()
{
	// Zuende?
	if(GetActTime() > iLifeTime)
		RemoveObject();
	if(GetActTime() > iLifeTime-SM4K_FadeTime) // Wir faden aus. Daher nix weiter mehr tun.
		return;

  // Rauchwolken vergrößern sich etwas...
  if(GetCon() < 70)
    DoCon(3);
  
  // Und werden langsamer... in Abhängigkeit zur Größe natürlich ;)
  Damp(GetCon());
	SetYDir(GetYDir(0,1000)+GetGravityAccel4K(500),0,1000);
  
  // Lebewesen können im Rauch nix sehen. >:D
  for(var obj in FindObjects(Find_Distance(GetCon()/2), Find_NoContainer(), Find_Or(Find_OCF(OCF_Living), Find_OCF(OCF_CrewMember)))) // <- NICHT OCF_Alive ... btw. muss Find_OCF doppelt?
  {
    if(!GetEffect("SmokeGrenade", obj))
      AddEffect("SmokeGrenade", obj, 25, 1, obj);
  }
}

public func FxSmokingTimer()
{
	// Und tolle Partikel-Effekte, damit Leute im Rauch auch nicht gesehen werden. ;)
	var alpha = BoundBy((GetActTime()-(iLifeTime-SM4K_FadeTime)) * 255 / SM4K_FadeTime, 0, 255);
  CreateParticle("SmokeGrenadeSmoke", 0, 0, 0, RandomX(-10, +10), GetCon()*10, RGBa(255, 255, 255, alpha));
  return 0;
}


/* der Rauch-Effekt */
global func FxSmokeGrenadeStart(object pTarget, int iEffectNumber, int iTemp)
{
  // KIs brauchen den Sichteffekt nicht.
  if(GetController(pTarget) == NO_OWNER) return 0;
  if(GetPlayerType(GetController(pTarget)) == C4PT_Script) return 0;
  
  EffectVar(0, pTarget, iEffectNumber) = ScreenRGB2(pTarget, RGBa(150, 150, 150, 254), 0, 0, false, SR4K_LayerSmoke);
  return 0;
}

global func FxSmokeGrenadeTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  var rgb = EffectVar(0, pTarget, iEffectNumber); // Haben wir denn einen Sichteffekt?
  if(!rgb) return 0;

  // Sind wir noch im Rauch?
  var smoked = false;
  for(var smoke in FindObjects(pTarget->Find_AtPoint(), Find_ID(SM4K)))
  {
    if(smoke->GetCon()/2 > ObjectDistance(smoke, pTarget))
    {
      smoked = true;
      break;
    }
  }
  
  if(smoked)
    rgb->DoAlpha(+10, 0, 254);
  else
  {
    rgb->DoAlpha(-10, 0, 254);
    if(rgb->GetAlpha() <= 0)
      return -1; // Boing. Wir werden nicht mehr gebraucht!
  }
  
  return 0;
}

global func FxSmokeGrenadeStop(object pTarget, int iEffectNumber, int iReason, bool fTemp)
{
  if(EffectVar(0,pTarget,iEffectNumber))
    EffectVar(0,pTarget,iEffectNumber)->RemoveObject();
}


/* Geschwindigkeits-Dämpfung */
public func Damp(int iStrength)
{
	iStrength = 1000 - iStrength;
  SetXDir(Mul4K(iStrength, GetXDir(this, 1000), 1000), this, 1000);
  SetYDir(Mul4K(iStrength, GetYDir(this, 1000), 1000), this, 1000);
}