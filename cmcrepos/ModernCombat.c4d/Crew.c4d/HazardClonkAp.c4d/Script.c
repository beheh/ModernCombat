/*-- Kampfclonk Appendto --*/

#strict
#appendto HZCK

local crosshair;


private func DeleteCrosshair()
{
}

/* neue Waffen-Steuerung */
/*public func UpdateCharge()
{
  // in Geb�uden/Fahrzeugen
  if(Contained())
    if(Contained()->~UpdateCharge(this()))
      return(1);

  var iVal=0, Content=Contents();
  
  if(Content)
    iVal = Content->~GetCharge();

  if(GetOwner() < -1) return();

  // HUD
  var hud = this()->~GetHUD();
  if(!hud) return();
  if(!Content) return(hud->Clear());
  if(Content->~IsWeapon())
  {
    var name = Content->GetFMData(FM_Name);
    if(Content->~IsWeapon2())
      if(Content->GetFMData(FT_Name))
        name = Format("%s|<c %x>(%s)</c>",name,RGB(177,177,177),Content->GetFMData(FT_Name));
    
    var ammoID = Content->GetFMData(FM_AmmoID);
    hud->Charge(iVal / 10);
    hud->Ammo(Content->GetAmmo(ammoID), Content->GetFMData(FM_AmmoLoad), name, true);
    hud->MasterAmmo(ammoID, this()->~AmmoStoring()->GetAmmo(ammoID));
    //if(Content->IsRecharging())
    if(Content->GetRecharge() < 100)
      hud->Recharge(Content->GetRecharge());
    else
      hud->Recharge();
  }
  else
  {
    hud->Clear();
    if(iVal)
      Content->~UpdateCharge(hud);
  }
    
  return(1);
}*/

/*public func WeaponAt(&x, &y, &r) {

  if(Contained()) return(0);

  var a = GetAction();
  var p = GetPhase();

  // x < 0: Richtung hinter den Clonk
  // x > 0: Richtung vor den Clonk
  x=0; y=0; r=0;

  // h�lt Waffe in der Hand
  if(a eq "WalkArmed") {
    x=0;
    y=1000;
    return(1);
  }
  
  else if(a eq "JumpArmed") {
    x=500-125*p;
    y=1000;
    return(1);
  }
  else if(a eq "SwimArmed") {
    x=4000;
    return(1);
  }
  //Jetpack
  else if(a eq "JetpackFlight") {
  
    x=4000;
    r=35;
    return(1);
  }

  // zielt mit der Waffe
  else if(IsAiming())
  {
    //if(p>4) r=(p-4)*11;
    //else    r=p*23-90;
    r = (Abs(crosshair->GetAngle())-90);
    x=-Sin(65+p*20,4000)-1500;
    y=-Sin(55+p*10,5000);
    if(a eq "AimLow")
      y += 4000;
    if(a eq "AimSquat")
    {
      x += 5000;
      y += 4500;
    }
    if(a eq "AimSquatLow")
    {
      x += 5000;
      y += 6000;
    }
    return(1);
  }
  // hat Waffe auf dem R�cken
  else
  {
    r=90;
    x=-3000;
    y=-2000;

    if(a eq "Walk") {
      y-=2000;
    }
    else if(a eq "Jump") {
      y-=2000;
    }
    else if(a eq "Scale" || a eq "ScaleDown") {
      x-=1000-Cos(p*45,250);
    }
    else if(a eq "Hangle") {
      x-=Cos(p*32,750);
    }
    else if(a eq "Push") {
      r=100;
      x+=1000-Cos(p*90,350);
      y-=2000;
    }
    else if(a eq "Swim") {
      r=135;
      x=-1000+Sin(p*22,500);
      y=-4000-Cos(p*22,250);
    }
    else if(a eq "Dive") {
      r=135+p*10;
      x=-1000;
      y=-4000;
    }

    return(1);
  }
}*/

/*public func WeaponAt(&x, &y, &r)
{

  var a = GetAction();
  var p = GetPhase();

  // x < 0: Richtung hinter den Clonk
  // x > 0: Richtung vor den Clonk
  x=0; y=0; r=0;

  // h�lt Waffe in der Hand
  if(a eq "WalkArmed")
  {
    x=0;
    y=1000;
    return(1);
  }
  
  else if(a eq "JumpArmed")
  {
    x=500-125*p;
    y=1000;
    return(1);
  }
  else if(a eq "SwimArmed")
  {
    x=4000;
    return(1);
  }
  //Jetpack
  else if(a eq "JetpackFlight")
  {
  
    x=4000;
    r=35;
    return(1);
  }
  // zielt mit der Waffe
  else if(this()->~IsAiming())
  {
    //if(p>4) r=(p-4)*11;
    //else    r=p*23-90;
    r = (Abs(crosshair->GetAngle())-90);
    x=-Sin(65+p*20,4000)-1500;
    y=-Sin(55+p*10,5000);
    if(a eq "AimLow")
      y += 4000;
    if(a eq "AimSquat")
    {
      x += 5000;
      y += 4500;
    }
    if(a eq "AimSquatLow")
    {
      x += 5000;
      y += 6000;
    }
    return(1);
  }
  // hat Waffe auf dem R�cken (vom Hazard-Team :D)
  else
  {
    r=90;
    x=-3000;
    y=-2000;

    if(a eq "Walk")
    {
      y-=2000;
    }
    else if(a eq "Jump")
    {
      y-=2000;
    }
    else if(a eq "Scale" || a eq "ScaleDown")
    {
      x-=1000-Cos(p*45,250);
    }
    else if(a eq "Hangle")
    {
      x-=Cos(p*32,750);
    }
    else if(a eq "Push")
    {
      r=100;
      x+=1000-Cos(p*90,350);
      y-=2000;
    }
    else if(a eq "Swim")
    {
      r=135;
      x=-1000+Sin(p*22,500);
      y=-4000-Cos(p*22,250);
    }
    else if(a eq "Dive")
    {
      r=135+p*10;
      x=-1000;
      y=-4000;
    }
    else return(0);

    return(1);
  }
}*/

func ResetShowWeapon(object pNew)
{
  var effect = GetEffect("ShowWeapon",this()); 
  if(!effect) return(false);
  
  if(pNew)
  {
    EffectVar(0, this(), effect) = GetID(pNew);
    EffectVar(1, this(), effect) = pNew;
  }
  else
  {
    EffectVar(0, this(), effect) = 0;
    EffectVar(1, this(), effect) = 0;
  }
  
  return(true);
}

public func DoAiming(int iChange)
{
  //zielen wir �berhaupt?
  if(!IsAiming())
    return();

  var angle = Abs(crosshair->GetAngle()) + iChange;

  // Winkel anpassen, wenn keine freie Auswahl (klassische Steuerung)
  if(!GetPlrCoreJumpAndRunControl(GetController()))
    angle = angle-angle%AIM_Step;
    
  /*if(ControlledCanAim())
  {
    angle = GetControlled()->SetAimAngle(angle);
  }
  else
  {*/
    // Winkel wird zu gro�?
    if(angle > AIM_Max || angle < 0)
      return();

    if(GetDir() == DIR_Left)
      angle = 360-angle;
  //}

  crosshair->SetAngle(angle);
  this()->~UpdateAiming();
}

/* HazardGear */
//Nicht jede Clonkart kann alles tragen. (z.B. Sanit�ter)

public func EquipGear(object pGear)
{
  // �berhaupt benutzbar
  if(!pGear) return(0);
  if(!(pGear->~IsHazardGear())) return(0);
  if(!HazardGearSupported(pGear)) return(0);
  return(_inherited(pGear));
}

public func HazardGearSupported(object pGear)//! -> Hazard 2.0
{
  return(true);//Standartm��ig alles schlucken.
}

private func AmmoBagContextCheck()
{
  return(!FindObject(NOAM));
}

/* Munition aus dem G�rtel nehmen */
protected func ContextAmmobag(object pCaller)
{
  [$AmmoBag$|Image=AMOC|Condition=AmmoBagContextCheck]
  var ammo, x;
  //Men� erzeugen
  CreateMenu(AMOC,this(),0,0,GetName(0,ABAG),0,1,1);
  
  var ambg = LocalN("ammobag");
  
  //Munition finden und hinzuf�gen
  while(ammo = Contents(x,ambg))
  {
    // nur Ammo
    if(!(ammo ->~IsAmmo())) continue;
    //hinzuf�gen
    AddMenuItem(GetName(ammo),"PackAmmo", GetID(ammo), this(), GetAmmo(GetID(ammo)),ObjectNumber(pCaller),GetDesc(ammo));
    //Z�hlervariable erh�hen
    x++;
  }
  return(1);
}

protected func PackAmmo(id idType, int iCaller)
{
  //Wir �bersehen mal, das beliebig viel Muni verpackt werden k�nnte.
  var pCaller = Object(iCaller);
  /*var pPack;
  
  for(var obj in FindObjects(Find_Container(),Find_ID(CUAM)))
  {
    if(obj->AmmoID() == idType)
    {
      pPack = obj;
      break;
    }
  }*/
  
  var pPack = CreateObject(CUAM,0,0,GetOwner());

  pPack->SetAmmoID(idType);
  var change = pPack->DoAmmoCount(GetAmmo(idType));
  DoAmmo(idType,-change);
  
  if(!Collect(pPack,this()))
  {
    DoAmmo(idType,+change); 
    RemoveObject(pPack);//What a fail. :C
  }
  else
    Sound("PackAmmo.ogg");
  
  if(this()->~AmmoBagContextCheck())
  {
    var sel = GetMenuSelection(pCaller);
    ContextAmmobag(pCaller);
    SelectMenuItem(sel,pCaller);
  }
}

/* Schaden */
public func OnHit(int iDmg, int iType, object pFrom)
{
  this()->~CalcPain(iDmg);
  this()->~HurtSounds(-iDmg,iType);
    
  return(_inherited(iDmg,iType,pFrom));
}

/* neues Zielsystem */
static const CH_ShowAlways = 1;

static const CH_MaxSpread = 500;

static const CH_WalkSpread = 80;
static const CH_JumpSpread = 140;
static const CH_ScaleSpread = 200;
static const CH_HangleSpread = 200;

static const CH_CrawlSpreadReduction = 2;
static const CH_AimSpreadReduction = 3;
static const CH_StandSpreadReduction = 4;

local crosshair,ammobag;
local spread, handr;

public func Initialize()
{
  InitCH();
  //if(Contained())
    //HideCH();
  return(_inherited());
}

/*func HandR()
{
  return(0);//return(handr/CH_Spread_Prec);
}*/

public func GetCrosshair()
{
  return(crosshair);
}

private func GetSpreadAOff()
{
  handr = RandomX(-spread/2/CH_Spread_Prec,+spread/2/CH_Spread_Prec);
  return(handr);
}

private func GetSpreadAOff2()
{
  handr = RandomX(-spread/2/CH_Spread_Prec,+spread/2/CH_Spread_Prec);
  return(handr*(100/CH_Spread_Prec));
}

private func TestSpread()
{
  var proc = GetProcedure();
  
  if(proc eq "WALK")
  {
    if(GetComDir())
      if(spread < CH_WalkSpread)
        return(CH_WalkSpread);
    return(0);
  }
  
  if(proc eq "SWIM")
  {
    if(spread < CH_WalkSpread)
      return(CH_WalkSpread);
    return(0);
  }
  
  if(proc eq "FLIGHT")
  {
    if(spread < CH_JumpSpread)
      return(CH_JumpSpread);
    return(0);
  }

  if((proc eq "SCALE")||(GetAction() eq "ScaleLadder"))
  {
    if(spread < CH_ScaleSpread)
      return(CH_ScaleSpread);
    return(0);
  }
  
  if(proc eq "HANGLE")
  {
    if(spread < CH_HangleSpread)
      return(CH_HangleSpread);
    return(0);
  }
  
  return(0);
}

public func UpdateCharge()//*ausnutz*
{
  /*var x,y,r;
  if(!this()->~WeaponAt(x,y,r))
  {
     HideCH();
     return(_inherited());
  }
    
  crosshair->SetAngle(r);*/
  
  //if(!ControlledCanAim())
  //{
    /*if(!c)
    {
      HideCH();
      return(_inherited());
    }*/
    
    if(!this()->~ReadyToFire() || !this()->~IsArmed() || (GetCursor(GetOwner()) != this()))
    {
      HideCH();
      return(_inherited());
    }
  //}
  
  var c = Contents();

  ShowCH();
  
  DoSpread(BoundBy(TestSpread()-spread,0,10));
  
  var unspread;
  if(c->~IsWeapon())
    unspread = c->GetFMData(FM_UnSpread);
  else
    unspread = c->~UnSpread();

    
  if(this()->~IsAiming())
  {
    if(this->~IsCrawling())
      DoSpread(-(CH_CrawlSpreadReduction+unspread));
    else
      DoSpread(-(CH_AimSpreadReduction+unspread));
  }
  else
  {
    DoSpread(-(CH_StandSpreadReduction+unspread));
  }

  this()->~UpdateAiming();
  
  return(_inherited());
}

global func AimAngleEx(int iMaxAngle, int iRange, bool bSpread)
{
  if(!this()) return(0);

  var angle = this()->~AimAngle2(iMaxAngle,iRange,bSpread);
  if(!angle)
    angle = this()->AimAngle(iMaxAngle,iRange,bSpread)*100;
    
  return(angle);
}

public func AimAngle2(int iMaxAngle, int iRange, bool bSpread)//Pr�zision 100. Also von 100 bis 36000.
{
   var angle;
   var x,y,r;
   this()->~WeaponAt(x,y,r);
   
   if(!this()->~IsAiming())
     angle = (90+r)*(GetDir()*2-1);
   else
     angle = crosshair->GetAngle();
  
   if(iRange)
   {
     var target = this()->~GetTarget(angle,iMaxAngle,iRange);
     if(target)
       angle = Angle(GetX(),GetY(),GetX(target),GetY(target));
   }
   
   if(bSpread)
     angle += GetSpreadAOff2();

   return(angle);
}

public func AimAngle(int iMaxAngle, int iRange, bool bSpread)
{
   var angle;
   var x,y,r;
   this()->~WeaponAt(x,y,r);
   
   if(!this()->~IsAiming())
     angle = (90+r)*(GetDir()*2-1);
   else
     angle = crosshair->GetAngle();
  
   if(iRange)
   {
     var target = this()->~GetTarget(angle,iMaxAngle,iRange);
     if(target)
       angle = Angle(GetX(),GetY(),GetX(target),GetY(target));
   }
     
   if(bSpread)
     angle += GetSpreadAOff();

   return(angle);
}

public func DoSpread(int iChange, int iMax)
{
  var wpn = Contents();
  if(!wpn) return();
  if(!wpn->~IsWeapon() && !wpn->~IsGrenade()) return();
  
  if(iMax) iChange = Max(0,BoundBy(spread+iChange,0,iMax)-spread);
  spread = BoundBy(spread+iChange,0,CH_MaxSpread);
  
  if(crosshair)
    crosshair->SetSpread(spread);
}

public func GetSpread()
{
  return spread;
}

private func InitCrosshair()
{
  InitCH();
}

public func InitCH()
{
  if(!crosshair)
    crosshair = CreateObject(CRH2,0,0,GetOwner());
  
  crosshair->Set(this());
  
  ResetCH();
}

public func ShowCH()
{
  if(!crosshair)
    InitCH();
  if(!crosshair)
    return();
	
  if(crosshair->Contained())
    crosshair->Exit();
    
  if(crosshair->GetActionTarget() != this())
    crosshair->Reset(this());
    
  if(!this()->~IsAiming())
  {
    //if((crosshair->GetAngle() != -90) || (crosshair->GetAngle() != +90))
      //ResetCH();
      
    if(GetDir() == DIR_Left)
      crosshair->SetAngle(-90);
    else
      crosshair->SetAngle(+90);
  }
}

public func ResetCH()
{
  if(!crosshair) return();
  crosshair->Reset(this());

  crosshair->SetSpread(spread);
  
  /*if(GetDir() == DIR_Left)
    crosshair->SetAngle(-90);
  else
    crosshair->SetAngle(+90);*/
  
  ScheduleCall(this(),"UpdateAiming",1);
}

public func HideCH()
{
  if(!crosshair) return();
  
  handr = 0;
  crosshair->UnSet();
}

public func IsAiming ()
{
  return(WildcardMatch(GetAction(), "Aim*"));
}

public func Entrance(object pContainer)
{
  HideCH();
  
  return(_inherited(pContainer));
}

protected func Departure(pContainer)      // Geb�ude verlassen
{
  UpdateCharge();
  return(_inherited(pContainer,...));
}

public func Destruction()
{
  if(crosshair)
    RemoveObject(crosshair);
  return(_inherited());
}

public func StartAiming()//Wegen fehlendem Hazard-Feature.
{
  if(Contained()) return(Contained()->~StartAiming());

  SetXDir(0);
  
  SetAction("Aim");
  
  // Aim low if possble
  if(Contents(0)->~GetFMData(FM_Aim) == 2 || Contents(0)->~GetFMData(FM_Aim) == -1)
  {
    if(GetActMapVal("Name","AimLow",GetID()))
      SetAction("AimLow");
  }
  else
  if(Contents(0)->~GetFMData(FM_Aim) == 3)
  {
    if(GetActMapVal("Name","AimSquat",GetID()))
      SetAction("AimSquat");
  }

  SetPhase(this()->~AimAngle2Phase(90));
  SetComDir(COMD_Stop);

  InitCrosshair();
  
  ScheduleCall(this(),"UpdateAiming",1);

  // Callback
  if(Contents(0)) Contents(0)->~AimStart();
}

public func StopAiming()
{
  if(crosshair)
  {
    ResetCH();
    if(GetDir() == DIR_Left)
      crosshair->SetAngle(-90);
    else
      crosshair->SetAngle(+90);
  }
  return(_inherited());
}

/*public func StartAiming()
{
  SetXDir(0);
  if(Contents(0)->~GetFMData(FM_Aim) == 2)
    SetAction("AimLow");
  else
    SetAction("Aim");
  SetPhase(4);
  SetComDir(COMD_Stop());

  ShowCH();//Sicherstellen das es auch angezeigt wird.

  // Callback
  if(Contents(0)) Contents(0)->~AimStart();
}

public func StopAiming()
{
  SetComDir(COMD_Stop());
  SetAction("Walk");
  this()->~CheckArmed();
  Sound("Grab");
 
  //Vertices zur�cksetzen
  SetVertex(0,0,0);
  SetVertex(0,1,0);
  
  if(CH_ShowAlways)
    ResetCH();
  else
    HideCH();
  
  // Callback
  if(Contents(0)) Contents(0)->~AimStop();
  return(1);
}

public func StartSquatAiming()
{
  // Abbrechen
  if(Contained()) return(1);
  if(GetAction() ne "WalkArmed" && GetAction() ne "Walk")
      return(1);

  SetXDir(0);
  if(Contents(0)->~GetFMData(FM_Aim) == 2)
    SetAction("AimSquatLow");
  else
    SetAction("AimSquat");
  SetPhase(4);
  SetComDir(COMD_Stop());

  ShowCH();

  // Callback
  if(Contents(0)) Contents(0)->~AimStart();
}

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

  var x,y,r;
  this()->~WeaponAt(x,y,r);
  //Position des Attachvertex so verschieben, dass das Kreuz von der Waffe aus ist
  //Henry:  Danke das ihr das nicht im Fadenkreuz macht! :D [edit] Naja, w�hr ja auch sch�n bl�d... -,-
  SetVertex(0,0,x/1000 * (GetDir()*2-1),0,2);
  SetVertex(0,1,y/1000,0,2);

  SetPhase(Min(6,7*Abs(a)/AIM_Max));
}*/

/*global func FxShowWeaponTimer(object pTarget, int iNumber, int iTime)
{
  // Waffe aktualisieren:
  var xoff, yoff, r;  // Offset, Winkel
  // kein Inventar oder falsche Aktion
  if(!Contents(0,pTarget)) {
    EffectVar(0, pTarget, iNumber) = 0;
    if(EffectVar(6, pTarget, iNumber)) {
      SetObjDrawTransform(1000,0,0,0,1000,0,EffectVar(6,pTarget,iNumber));
      EffectVar(6, pTarget, iNumber) = 0;
    }
    SetGraphics(0, pTarget, 0, WeaponDrawLayer);
    return(FX_OK);
  }
  //Die Waffe momentan �berhaupt anzeigen?
  if(!(pTarget->~WeaponAt(xoff, yoff, r))) {
    EffectVar(0, pTarget, iNumber) = 0;
    if(EffectVar(6, pTarget, iNumber)) {
      SetObjDrawTransform(1000,0,0,0,1000,0,EffectVar(6,pTarget,iNumber));
      EffectVar(6, pTarget, iNumber) = 0;
    }
    SetGraphics(0, pTarget, 0, WeaponDrawLayer);
    return(FX_OK);
  }
  var obj = Contents(0,pTarget), id=GetID(obj);
  // Waffe nicht mehr aktuell
  if(EffectVar(0, pTarget, iNumber) != id) {
    // neues Objekt ist Waffe, oder ein Objekt, das gezeichnet werden soll
    if(obj->~IsWeapon() || obj->~IsDrawable()) {
      EffectVar(0, pTarget, iNumber) = id;
      EffectVar(6, pTarget, iNumber) = obj;
      SetGraphics(0, pTarget,id, WeaponDrawLayer, GFXOV_MODE_Object,0,GFX_BLIT_Parent,obj);
    }
    // neues Objekt ist keine Waffe
    else {
      EffectVar(0, pTarget, iNumber) = 0;
      if(EffectVar(6, pTarget, iNumber)) {
        SetObjDrawTransform(1000,0,0,0,1000,0,EffectVar(6,pTarget,iNumber));
        EffectVar(6, pTarget, iNumber) = 0;
      }
      SetGraphics(0, pTarget, 0, WeaponDrawLayer);
      return(FX_OK);
    }
  }

  id = EffectVar(0, pTarget, iNumber);
  obj = EffectVar(6, pTarget, iNumber);

  // Ausrichtung nach Blickrichtung des Clonks
  // Variablen f�r die Transformation

  var width, height;  // Breiten- und H�henverzerrung der Waffe
  var xskew, yskew;   // Zerrung der Waffe, wird zur Rotation gebraucht
  var size;           // Gr��e der Waffe in der Hand: 1000 = 100%
  // Variablen f�r die Position
  var xaim, yaim;     // Offset, dass sich durch zielen ergibt
  var dir;            // Richtung in die das Objekt schaut
  
  //schnell noch Rotation dazurechnen oder so!
  r += ObjectCall(obj,"HandR");
  r += ObjectCall(pTarget,"HandR");
  
  // Variablen mit Werten versehen
  width = height = xskew = yskew = 1;
  size = id->~HandSize();
  if(!size) size = 1000;
  dir  = GetDir()*2-1;
  if(r > 180 || r < -180)
    dir *= -1;
  r *= dir;

  var xfact = size * ObjectCall(obj,"HandX");    // Attachpunkte dazurechnen
  var yfact = size * ObjectCall(obj,"HandY");

  xoff += Cos(r,xfact)/1000 + dir*Sin(r,yfact)/1000;
  yoff -= Cos(r,yfact)/1000 - dir*Sin(r,xfact)/1000;

  if(dir == 1) 
  {
    height = -1;
    xskew = -1;
    yskew = -1;
  }

  r = -90*dir-r-90;
  height *= width *= Cos(r, size);
  xskew *= Sin(r, size);
  yskew *= -xskew;
  xoff *= dir;

  SetObjDrawTransform(1000,xskew,xoff,yskew,1000,yoff, pTarget, WeaponDrawLayer); //position
  SetObjDrawTransform(width,xskew,0,yskew,height,0, obj); //Gr��e und Rotation

  // abspeichern, damit abrufbar
  r = -r-90;
  var w = GetDefCoreVal("Width",0,id)/2;
  var brly = DefinitionCall(id,"BarrelYOffset");
  var brlx = DefinitionCall(id,"BarrelXOffset");
  var r2 = (Angle(0,0,w-brlx/1000,brly/1000)-90)*dir;
  var dist = Distance(0,0,w*1000-brlx,brly);
  //Log("%d - %d - %d - %d",w,brl,r2,dist);
  EffectVar(1, pTarget, iNumber) = r;
  EffectVar(2, pTarget, iNumber) = xoff-Sin(r,size*w);
  EffectVar(3, pTarget, iNumber) = yoff+Cos(r,size*w);
  EffectVar(4, pTarget, iNumber) = xoff+Sin(r+r2,size*(dist))/1000;
  EffectVar(5, pTarget, iNumber) = yoff-Cos(r+r2,size*(dist))/1000;
  //EffectVar(4, pTarget, iNumber) = xoff+Sin(r,size*(w));	
  //EffectVar(5, pTarget, iNumber) = yoff-Cos(r,size*(w));
  //Log("%d / %d",EffectVar(4, pTarget, iNumber),EffectVar(5, pTarget, iNumber));
}*/

public func SetAiming(int iAngle)
{
  //zielen wir �berhaupt?
  if(!this()->~IsAiming())
    return();

  // Winkel anpassen, wenn keine freie Auswahl (klassische Steuerung)
  if(!GetPlrCoreJumpAndRunControl(GetController()))
    iAngle = iAngle-iAngle%AIM_Step;

  // Winkel wird zu gro�?
  if(iAngle > AIM_Max || iAngle < 0)
    return();

  if(GetDir() == DIR_Left)
    iAngle = 360-iAngle;
  crosshair->SetAngle(iAngle);

  this()->~UpdateAiming();
}

//Kill-Nachrichten
private func DeathAnnounce(int plr, object clonk, int killplr)
{
  if(GetEffect("NoAnnounce", this)) return();

  if(killplr == -1)
    return(/*this()->CLNK::DeathAnnounce(plr,clonk,killplr)*/);
    
  if(!clonk)
    clonk = this();//:o
  if(!clonk) return();
  
  //Selfkill?
  if(plr == killplr)
    KILL->SKMsg(plr, clonk);
  else
    KILL->KTMsg(killplr, plr, clonk);

  KILL->KillStat(GetCursor(killplr),plr);//hier auch clonk->~KillIcon()? k�nnte lustig sein :>
  
  //this()->CLNK::DeathAnnounce(plr,clonk,killplr);
}

/* Nahkampfsystem */
private func Fighting()
{
  if((GetCursor(GetOwner()) != this()) || (GetPlayerType(GetController()) == C4PT_Script))//Rudiment�re KI Selbstverteidigung.
  {
    if(!Random(2))
     ControlThrow();
    else if(!Random(2))
      ControlDig();
  }
  return(1);
}

local punchtype;//<-lol

private func Punching()
{
  if (!Random(3)) Sound("Kime*");
  Sound("Punch*");
  
  if(punchtype == 0)
  {
    Punch(GetActionTarget());
  }
  else if(punchtype == 1)
  {
    if(GetAction(GetActionTarget()) ne "Punch")
    {
      Fling(GetActionTarget(), 4 * GetDir() - 2, -1);
      Punch(GetActionTarget(),10);
    }
    else
      Fling(this(), 0, -1);//Schwupps. Selber hingefallen.
  }

  SetComDir(COMD_Stop);

  return(1);
}

public func InCloseCombat()
{
  if((GetProcedure() eq "FIGHT") && GetActionTarget())//ToDo: Wirklich? :o
    return(true);
  return(false);
}

public func ControlThrow()
{
  if(!InCloseCombat()) return(_inherited(...));
  
  if(GetAction() ne "Fight")//Nicht in Ausgangsstellung?
    return(1);
    
  if(!Random(3)) Sound("Kime*");
  punchtype = 0;
  SetAction("Punch");
  
  return(1);
}

public func ControlThrowSingle()
{
  if(InCloseCombat()) return(ControlThrow(...));
  return(_inherited(...));
}

public func ControlThrowDouble()
{
  if(InCloseCombat()) return(ControlThrow(...));
  return(_inherited(...));
}

public func ControlDig()
{
  if(!InCloseCombat()) return(_inherited(...));
  
  if(GetAction() ne "Fight")//Nicht in Ausgangsstellung?
    return(1);
    
  if (!Random(3)) Sound("Kime*");
  punchtype = 1;
  SetAction("Punch");

  return(1);
}

public func ControlDigSingle()
{
  if(InCloseCombat()) return(ControlDig(...));
  return(_inherited(...));
}

public func ControlDigDouble()
{
  if(InCloseCombat()) return(ControlDig(...));
  return(_inherited(...));
}

public func ControlSpecial()
{
  [$CtrlInventoryDesc$|Image=INVT]
  
  if(Contained())
  {
    if(Contained()->~ContainedSpecial(this()))
      return(1);
  }
  
  return(_inherited(...));
}

/* Support f�r Gesch�tze und Co. */

/*
public func GetControlled()
{
  var ctrl = Contained();
  if(!ctrl && (GetProcedure() eq "PUSH"))
    ctrl = GetActionTarget();
    
  return(ctrl);
}

public func ControlledCanAim()
{
  var ctrl = Contained();
  if(!ctrl && (GetProcedure() eq "PUSH"))
    ctrl = GetActionTarget();

  if(ctrl)
    return(ctrl->~UserCanAim());
}

protected func ControlUpdate(object clonk, int comdir, bool dig, bool throw)
{
  if(this()->~Control2Contents("ControlUpdate", comdir, dig, throw)) return(1);
  if(this()->~ControlLadder("ControlUpdate", comdir, dig, throw)) return(1);

  if(IsAiming() || ControlledCanAim())
  {
    AimUpdate(this(), comdir, 1, "ControlConf");
    return(1);
  }

  return(_inherited());
}

protected func ControlUp()
{
  this()->~ClearMacroCommands();
  this()->~SetAggroLevel(0,0,0,0,"ControlUp");

  if(IsAiming() || ControlledCanAim())
  {
	  AimUp(this(), 1, "ControlConf");
      return(1);
  }
  if (this()->~Control2Contents("ControlUp") ) return(1);
  if (this()->~ControlLadder("ControlUp") ) return(1);
  return(_inherited());
}

protected func ControlUpDouble()
{
  if(IsAiming() || ControlledCanAim())
  {
	  AimUp(this(), 1, "ControlConf");
      return(1);
  }
  if (this()->~Control2Contents("ControlUpDouble") ) return(1);
  if (this()->~ControlLadder("ControlUpDouble") ) return(1);
  return(_inherited());
}

protected func ControlDown()
{
  this()->~ClearMacroCommands();
  this()->~SetAggroLevel(0,0,0,0,"ControlDown");

  if(IsAiming() || ControlledCanAim())
  {
	  AimDown(this(), 1, "ControlConf");
	  return(1);
  }
  if (this()->~Control2Contents("ControlDown") ) return(1);
  if (this()->~ControlLadder("ControlDown") ) return(1);
  if(Contents(0))
    if (GetPlrDownDouble(GetOwner())
      && !GetEffect("SquatAimTimeout")
      && (Contents(0)->~CanAim()) && !IsAiming()
      && (GetAction() eq "WalkArmed" || GetAction() eq "Walk"))
      {
         StartSquatAiming(); 
         return(1);
      }
  return(_inherited());
}

protected func ControlThrow()
{
  this()->~ClearMacroCommands();
  this()->~SetAggroLevel(0,0,0,0,"ControlThrow");

  // Bei vorherigem Doppel-Stop nur Ablegen  
  if ( GetPlrDownDouble(GetOwner()) )
  {
    AddEffect("SquatAimTimeout", this(), 1, 15, this());
    return(inherited());
  }
  // Steuerung an gerittenes Objekt weitergeben
  if(IsRiding())
    if(GetActionTarget()->~ControlThrow(this()))
      return(1);
  //Steuert man grade was?
  if(ControlledCanAim())
    if(GetControlled()->ControlThrow(this()))
      return(1);
  // Steuerung an Inhalt weitergeben
  if (this()->~Control2Contents("ControlThrow") ) return(1);
  // Oder Leiter und �hnliches
  if (this()->~ControlLadder("ControlThrow") ) return(1);
  // Sonst internen Throw-Befehl ausführen
  return(inherited());
}*/