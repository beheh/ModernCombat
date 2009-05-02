/*--- Der Peacemaker ---*/

#strict
#include HZCK

#include L_A2 // has cmc agility
#include L_ND // can use grenades
#include L_CR // can crawl
#include L_SK // uses skins

public func SkinCount() { return 2; }

public func WeaponCollectionLimit() { return(3); }
public func ObjectCollectionLimit() { return(2); }

public func CanUse(id idObj)
{
  if(idObj == HARM) return(0);
  return(_inherited(idObj,...));
}

/*private func DolphinJump()
{
  Message("DolphinJump()",this());
  return();
  //if(WildcardMatch(GetAction(),"Swim*"))
  //if(GetProcedure() eq "SWIM")
    //return(_inherited());
}
  
public func JumpStart(bool bBackflip)
{
  _inherited(bBackflip,...);
  if(GetAction() eq "Dive")//Hack! :P
    SetAction("Jump");
}*/

private func AbortWalk()
{
  if(GetAction() eq "Dive")
  {
    var c = Contents();
    if(c)
    {
      if((c->~IsWeapon() && !c->~GetFMData(FM_Aim)) || c->~IsGrenade())
        SetAction("Jump");
    }
  }
}

private func CheckArmed()
{
  if(GetAction() eq "Swim") return();
  //if (GetAction() eq "Dive") if (IsArmed2()) return(SetAction("JumpArmed"));
  
  return(_inherited());
}

protected func ContactBottom()
{
  return(0);
}


/*
public func UpdateAiming()
{
  var a = crosshair->GetAngle();

  if((this()->~IsAiming()) || CH_ShowAlways)// || ControlledCanAim())
  {
    if(!crosshair) ShowCH();

    if((GetDir() == DIR_Left && a > 0) ||
      (GetDir() == DIR_Right && a < 0))
    {
      crosshair->ChangeDir();//*hrhrhr*
      //if(ControlledCanAim())
        //GetControlled()->SetDir(GetDir()*2-1);
    }
  }

  if(!this()->~IsAiming())
    return();

  if(GetAction() ne "AimCrawl")
  {
    var x,y,r;
    this()->~WeaponAt(x,y,r);
    SetVertex(0,0,x/1000 * (GetDir()*2-1),0,2);
    SetVertex(0,1,y/1000,0,2);

    SetPhase(Min(6,7*Abs(a)/AIM_Max));
  }
}

protected func ControlCommand(szCommand, pTarget, iTx, iTy, pTarget2, Data) 
{
  ClearMacroCommands();
  SetAggroLevel(0,0,0,0,"ControlCommand");

  // Context -> Keine Sonderbehandlung 
  if(szCommand S= "Context") { return(0); }

  // Kommando MoveTo in Schießen umwandeln
  if (szCommand S= "MoveTo")
  { 
    if(IsAiming())
    {
      var iAngle = Angle(GetX(),GetY(),iTx,iTy);
      if(iAngle >180)
        iAngle -= 360;
      // auskommentiert: Mit Maus Zielen ist nun supergenau :)
      //iAngle -= iAngle%AIM_Step;
      
      if(iAngle > 0)
        SetDir(DIR_Right);
      else
        SetDir(DIR_Left);

      //Winkel wird zu groß?
      if(GetAction() ne "AimCrawl")
      {
        iAngle = BoundBy(iAngle,-AIM_Max,+AIM_Max);
      }
      else
      {
        iAngle = BoundBy(iAngle,-(90+CRAWL_AIM_Max),+(90+CRAWL_AIM_Max));
      }
      iAngle = BoundBy(iAngle,-AIM_Max,+AIM_Max);

      crosshair->SetAngle(iAngle);
      UpdateAiming();
      //Wichtig: Waffe updaten
      EffectCall(this(),wpneffect,"Timer");

      // Fertig, Feuern!
      if(!(Contents()->IsRecharging()))
        Control2Contents("ControlThrow");
      return(1);
    }
    // Klettert an einer Leiter
    if(GetAction() eq "ScaleLadder")
      ClimbLadder();
  }
  // Sonst nicht abfangen 
  return(_inherited(szCommand, pTarget, iTx, iTy, pTarget2, Data) ); 
}

public func DoAiming(int iChange)
{
  //zielen wir überhaupt?
  if(!IsAiming())
    return();

  var angle = Abs(crosshair->GetAngle()) + iChange;

  // Winkel anpassen, wenn keine freie Auswahl (klassische Steuerung)
  if(!GetPlrCoreJumpAndRunControl(GetController()))
    angle = angle-angle%AIM_Step;

  // Winkel wird zu groß?
  if(GetAction() ne "AimCrawl")
  {
    if(angle > AIM_Max || angle < 0)
      return();
  }
  else
  {
    if(angle > +(90+CRAWL_AIM_Max) || angle < (90-CRAWL_AIM_Max))
      return();
  }

  if(GetDir() == DIR_Left)
    angle = 360-angle;
  crosshair->SetAngle(angle);
  UpdateAiming();
}



local spread;
static const CH_CrawlSpread = 130;

private func TestSpread()
{
  var val = inherited();
  if(val) return(val);

  var a = GetProcedure();
  
  if(WildcardMatch(GetAction(),"Crawl*"))
  {
    if(GetComDir())
      if(spread < CH_CrawlSpread)
        return(CH_CrawlSpread);
    return(0);
  }
}

protected func ControlContents(idTarget)
{
  // Aktuelles Objekt verhindert Shift?
  if(Contents())
    if(Contents()->~RejectShift(idTarget))
      return(1);
      
  // Zielobjekt will gar nicht, dass wir was machen
  if(idTarget->~NoControlContents()) return();
  
  // Wer ist idTarget?
  var pTarget = FindContents(idTarget);
  
  // Zielaktion abbrechen (Spezial: außer wenn mit anwählbarem 
  // Objekt auch gezielt werden kann...)
  if(this()->~IsAiming())
  {
     var phase = GetPhase();
     // Zielaktion anpassen
     if(pTarget->~IsWeapon())
     {
       if(!IsCrawling())
       {
         if(pTarget->~GetFMData(FM_Aim) == 2)
           SetAction("AimSquatLow");
         else
           SetAction("AimSquat");
           
         SetPhase(phase);
       }
     }
     else if(pTarget->~IsGrenade())
     {
       SetAction("AimSquat");
       SetPhase(phase);
     }
     else
     {
       this()->~StopAiming();
     }
  }
  // Hast du noch einen letzten Wunsch, Contents(0)??!
  if(Contents(0)) Contents(0)->~Deselection(this());
  // Rotieren
  if (!ShiftContents(0, 0, idTarget)) return(1);
  // Waffe ziehen/wegstecken
  this()->~CheckArmed();
  // Munitionsanzeige updaten
  UpdateCharge();
  // Objekt benachrichtigen
  if(Contents(0)) Contents(0)->~Selection(this());
}
*/