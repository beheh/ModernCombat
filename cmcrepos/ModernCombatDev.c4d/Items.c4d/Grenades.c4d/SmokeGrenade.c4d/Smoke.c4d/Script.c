/*-- Rauch --*/

#strict

local a,lifetime,max_size;

///Spezialisierung

private func TimerCall()
{
  for(var obj in FindObjects (Find_OCF(OCF_CrewMember),Find_Distance(GetObjHeight()/2/10*9)))
  {
    if(GetPlayerType(GetOwner(obj)) == C4PT_Script) continue;
    
    if(!GetEffect("SmokeGrenade",obj))
      AddEffect("SmokeGrenade",obj,25,1,obj);
  }
}

public func GetAlpha() {return(a);}

//toller Effekt
global func FxSmokeGrenadeStart(object pTarget, int iEffectNumber, int iTemp)
{
  EffectVar(0,pTarget,iEffectNumber) = ScreenRGB2(pTarget,RGB(150,150,150),4,0,false,SR4K_LayerSmoke);
  return(1);
}

global func FxSmokeGrenadeTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  var rgb = EffectVar(0,pTarget,iEffectNumber);
  if(!rgb) return(-1);//*plopp*
  
  var alpha = 255;
  for(var smoke in FindObjects(pTarget->Find_AtPoint(),Find_ID(SM4K)))
  {
    if(smoke->GetAlpha() < alpha)
      alpha = smoke->GetAlpha();
  }
  
  if(alpha < rgb->GetAlpha())
    rgb->DoAlpha(+14,000,alpha);
    
  if(alpha > rgb->GetAlpha())
    rgb->DoAlpha(-14,alpha,255);
}

global func FxSmokeGrenadeStop(object pTarget, int iEffectNumber, int iReason, bool fTemp)
{
  if(EffectVar(0,pTarget,iEffectNumber))
    EffectVar(0,pTarget,iEffectNumber)->RemoveObject();
}


///(Stark) modifizierte Grundfunktionen von GS4K(Gas-Grundobjekt).

func Initialize()
{
  SetCon(5);
  lifetime = 35*4 + Random(35*6);
  max_size = 60+Random(39);

  SetAction("Be");
  SetClrModulation(RGBa(255,255,255,0));
}

func Timer()
{
  TimerCall();//Relikt aus original Gas-Objekt.
  
  //Dämpfen.
  SetXDir(GetXDir(0,100)-(GetCon()*GetXDir(0,100)/120),0,100);
  SetYDir(GetYDir(0,100)-(GetCon()*GetYDir(0,100)/120),0,100);

  if(!Stuck() && !GetContact())
    SetCon(BoundBy(GetCon()+4,1,max_size));
    
  if(Stuck())//Steckt fest?
    SetCon(BoundBy(GetCon()-3,1,max_size));//Mal sehen ob wir durch eine Schrumpfkur etwas Platz bekommen.
  else
    Spread();
    
  if(!this()) return();//Sicherheits Check.
  
  SetYDir(GetYDir(0,100)+30,0,100);//Das Gas hier fällt auf dem Boden.
  
  if(GetActTime() >= lifetime)
  {
    a = BoundBy(a+1,0,255);
    SetClrModulation(RGBa(255,255,255,a));
    if(a >= 255) RemoveObject();
  }
}

func Spread()
{
  var contact = GetContact(this(),-1);
  if(!contact) return();
  
  if(contact & CNAT_Bottom)
    SetYDir(GetYDir(0,100)-30,0,100);
  if(contact & CNAT_Top)
    SetYDir(GetYDir(0,100)+30,0,100);
  if(contact & CNAT_Right)
    SetXDir(GetXDir(0,100)-30,0,100);
  if(contact & CNAT_Left)
    SetXDir(GetXDir(0,100)+30,0,100);
}

public func Accelerate(int iX,int iY,object pObject)
{         
  SetXDir(Mul4K(iX,GetXDir(pObject,100),100),pObject,100);
  SetYDir(Mul4K(iY,GetYDir(pObject,100),100),pObject,100);
}

public func Damp(int iStrength,object pObject)
{         
  Accelerate(iStrength,iStrength,pObject);
}

//Contact-Calls (Wegen Ausbreitung.)
protected func ContactBottom()
{
  Spread();
  if(VerticesStuck() == 1)
    AutoUnstuck(this(),0,+1);
}

protected func ContactTop()
{
  Spread();
  if(VerticesStuck() == 1)
    AutoUnstuck(this(),0,-1);
}

protected func ContactLeft()
{
  Spread();
  if(VerticesStuck() == 1)
    AutoUnstuck(this(),-1,0);
}

protected func ContactRight()
{
  Spread();
  if(VerticesStuck() == 1)
    AutoUnstuck(this(),+1,0);
}