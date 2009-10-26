/*-- Clonk Appendto --*/

#strict 2
#appendto CLNK


/* Allgemein */

//damit normale Clonks auch Waffen etc. nutzen können -> in Zivilclonk (hust) umwandeln
protected func Construction()
{
  if(GetID() == CLNK)
  {
    if(!this->~Redefine2(CIVC))
      if(!this->~Redefine(CIVC))
        if(GetOCF(this) & OCF_Alive)
        {
          ChangeDef(CIVC,this);
          ObjectSetAction(this,"Walk");
        }
        
    return this->Construction(...);
  }
  
  return _inherited(...);
}

protected func Initialize()
{
  if(!GetEffect("DmgCheck",this))
    AddEffect("DmgCheck",this,1,0);
  
  return _inherited(...);
}

public func IsThreat() { return true; }

public func MaxContentsCount() { return 3; }//Normale Clonks können mehr tragen. :>


/* Schmerz-System */
local iPain, iPainFactor, pRedHurt;

public func PainFactor(int iFactor)
{
  if(iFactor) iPainFactor = iFactor;
  return iPainFactor;
}

public func DoPain(int iChange)
{
  return SetPain(GetPain()+iChange);
}

public func GetPain()
{
  return iPain;
}

public func SetPain(int iValue)
{
  return;//Deaktiviert.

  //Allgemein
  var iOld = iPain;
  var inew = BoundBy(iValue,0,100);
  iPain = inew;
  //Message("%d/100 Schmerzen",this,iPain);
  
  //Effekte
  if((iOld < inew) && !HasBlackOut())//Mehr Schmerzen?
  {
    if(inew >= 100)
      AddEffect("BlackOut",this,10,20,this);
    else
    if(!GetEffect("Pain",this))
      AddEffect("Pain",this,10,10,this);
      
    if(inew >= 30)
    {
      var effect = GetEffect("Shock",this);
      if(!effect)
        effect = AddEffect("Shock",this,10,0,this);
      else
        EffectCall(this,effect,"Update"); 
    }
  }
  
  //Einfärbung
  var a = 255-Cos(BoundBy(900*iPain/100,0,900),255,10);
  if(a > 0)
  {
    pRedHurt = ScreenRGB3(this,RGB(a,0,0),true,SR4K_LayerRedHurt,pRedHurt);
  }
  else
  {
    if(pRedHurt) RemoveObject(pRedHurt);
  }
  
  return iPain;
}

public func CalcPain(int iDmg)
{
  DoPain((iDmg*80/(GetPhysical("Energy",0)/1000))*(100+iPainFactor)/100);
  iPainFactor = 0;
}

public func FxPainStart(pTarget,iEffectNumber)
{
  //...
}

public func FxPainTimer(pTarget,iEffectNumber,iEffectTime)
{
  if(GetPain() > 0)
    DoPain(-1);
  else
    return -1;
}

public func FxPainStop(object pTarget, int iEffectNumber, int iReason, bool fTemp)
{
  //...
}

public func HasBlackOut()//Später solls wieder FakeDeath heissen.
{
  return GetEffect("BlackOut",this);
}

public func FxBlackOutStart(pTarget,iEffectNumber)
{
  SetComDir(COMD_Stop,pTarget);
  pTarget->Sound("ClonkDie*.ogg");
  if(!ObjectSetAction(pTarget,"Death",0,0,1))
    pTarget->SetAction("Dead");
  
  var pHelper = pTarget->CreateObject(FKDT,0,0,GetController(pTarget));
  pHelper->Set(pTarget);
  EffectVar(1,pTarget,iEffectNumber) = pHelper;
  
  RemoveEffect("Pain",this);
}

public func FxBlackOutTimer(pTarget,iEffectNumber,iEffectTime)
{
  if(GetPain() > 50)
    DoPain(-1);
  else
    return -1;
}

public func FxBlackOutStop(object pTarget, int iEffectNumber, int iReason, bool fTemp)
{
  if(EffectVar(1,pTarget,iEffectNumber))
    RemoveObject(EffectVar(1,pTarget,iEffectNumber));
  
  if(iReason == 4)
    FadeOut(pTarget);
    
  if(iReason == 0)
  {
    AddEffect("Pain",this,10,10,this);
    ObjectSetAction(pTarget,"FlatUp",0,0,1);
  }
}


static const PAIN_ShockOffset = 25;

public func FxShockStart(pTarget,iEffectNumber,tmp)
{
  EffectVar(0,pTarget,iEffectNumber) = pTarget->GetPain();
}

public func FxShockUpdate(pTarget,iEffectNumber)
{
  var start = EffectVar(0,pTarget,iEffectNumber);
  
  Log("Shock: %d",PAIN_ShockOffset-(pTarget->GetPain()-start));

  if((pTarget->GetPain()-start) < -PAIN_ShockOffset)
    return -1;

  if((pTarget->GetPain()-start) > PAIN_ShockOffset)
  {
    pTarget->SetAction("Tumble");
    return -1;
  }
}

public func FxShockStop(object pTarget, int iEffectNumber, int iReason, bool fTemp)
{
  //...
}


/* FakeDeath */

global func PauseFakeDeath(bool fPause, object pTarget)
{
  return;
}

global func FakeDeath(object pTarget)
{
  return;
}

global func StopFakeDeath(object pTarget)
{
  return;
}

global func IsFakeDeath(object pTarget)
{
  return;
}


/* FEUAAAA!!!! */
public func Incineration()
{
  Extinguish();
  if(GetAction() == "Death") return 0;//Fakedeath
  Schedule("DoDmg(5,DMG_Fire,0,1)",1,20,this);
  AddFireEffect(this,30,FIRE_Red,1);
}

public func OnHit(int iDmg, int iType, object pFrom)
{
  if(!GetAlive() || IsFakeDeath()) return;

  CalcPain(iDmg);
  HurtSounds(-iDmg,iType);
  
  if(iType == DMG_Bio)
    AddFireEffect(this,10,RGB(20,255,30));
    
  return _inherited(iDmg,iType,pFrom);
}

public func HurtSounds(int iDmg, int iType)
{
  iDmg = -iDmg;
  if(iDmg <= 0) return;

  //Projectile
  if(iType == DMG_Projectile)
  {
    if(!Random(BoundBy(12-iDmg,0,12)))
      Sound("ClonkPain*.ogg");
    return;
  }
  //Flame
  if(iType == DMG_Fire)
  {
    if(!Random(BoundBy(20-iDmg,0,20)))
      Sound("ClonkBurn*.ogg");
    return;
  }
  //Explosion
  if(iType == DMG_Explosion)
  {
    if(!Random(BoundBy(2-iDmg,0,2)))
      Sound("ClonkPain*.ogg");
    return;
  }
  //Energy
  if(iType == DMG_Energy)
  {
    if(!Random(BoundBy(10-iDmg,0,10)))
      Sound("ClonkPain*.ogg");
    return;
  }
  //Bio
  if(iType == DMG_Bio)
  {
    if(!Random(BoundBy(20-iDmg,0,20)))
      Sound("ClonkPoisened*.ogg");
    return;
  }
  
  if(!Random(BoundBy(10-iDmg,0,10)))
    Sound("ClonkPain*.ogg");
}


/* Reject Shift */
protected func ControlContents(idTarget)
{
  if(Contents())
    if(Contents()->~RejectShift(idTarget))
      return 1;

  return _inherited(idTarget,...);
}

/* Fallschaden */
func Hit2(int xDir, int yDir)
{
  var hit = Distance(xDir,yDir);//Max(xDir,yDir);

  if(hit >= 800)
    Sound("ClonkImpact*.ogg");
  else if(hit >= 600)
    Sound("ClonkFall*.ogg");

  if(!FindObject(FDMG)) return _inherited(xDir,yDir,...);
	if(!GetAlive(this)) return _inherited(xDir,yDir,...);
  
  if(hit <= 700) return _inherited(xDir,yDir,...);
  DoDmg((hit-700)*2/10,DMG_Melee,this);
  
  if(GetAlive(this))
    Sound("ClonkPain*.ogg");
  
  return _inherited(xDir,yDir,...);
}


/* Killverfolgung */

local killicon;
local LastDmgType;

public func LastDamageType(int type)
{
  if(type)
    LastDmgType = type;
  
  return LastDmgType;
}

public func KillIcon(id idKillIcon)
{
  if(idKillIcon)
    killicon = idKillIcon;
    
  return killicon;
}

private func DeathAnnounce(int plr, object clonk, int killplr)
{
  if(killplr == -1)
    return _inherited(plr,clonk,killplr);
  
  //Selfkill?
  if(plr == killplr)
    KILL->SKMsg(plr, clonk);
  else
    KILL->KTMsg(killplr, plr, clonk);

  KILL->KillStat(GetCursor(killplr),plr);//hier auch clonk->~KillIcon()? könnte lustig sein :>
  
  inherited(plr,clonk,killplr);
}


/* Steuerungs-Callbacks */
protected func ControlSpecial()
{
  if (Contained())
  {
    if(Contained()->~ContainedSpecial(this))
      return 1;
  }
  
  return _inherited(...);
}

protected func ControlSpecial2()
{
  [$CtrlMenuDesc$|Image=CXTX]
 
  // In einem Gebäude oder Fahrzeug: das Kontextmenü des Gebäudes öffnen
  if (Contained())
  {
    if(Contained()->~ContainedSpecial2(this))
      return 1;
    if((Contained()->GetCategory() & C4D_Structure) || (Contained()->GetCategory() & C4D_Vehicle))
		{
      var v = SetCommand(this,"Context",0,0,0,Contained());
			ExecuteCommand();
			return v;
		}
  }
  // Fasst ein Objekt an: Kontextmenü des angefassten Objekts öffnen
  if(GetAction() == "Push")
	{
	  var r = SetCommand(this,"Context",0,0,0,GetActionTarget());
		ExecuteCommand();
		return r;
	}
  // Trägt ein Objekt: Kontextmenü des ersten getragenen Objekts öffnen
  if(Contents(0))
	{
		var v = SetCommand(this,"Context",0,0,0,Contents(0));
		ExecuteCommand();
		return v;
	}
  // Ansonsten das Kontextmenü des Clonks öffnen
  var v = SetCommand(this,"Context",0,0,0,this);
	ExecuteCommand();
	return v;
}


/* Hazard-Heilung */

public func StopHealing()
{
  SetComDir(COMD_Stop);
  SetAction("Walk");
  Sound("Grab");
  while(GetEffect("*Heal*",this))
    RemoveEffect("*Heal*",this);
}

// Heilt sich mit einem Medikit o.ä.
public func IsHealing () { return WildcardMatch(GetAction(), "Heal*"); }