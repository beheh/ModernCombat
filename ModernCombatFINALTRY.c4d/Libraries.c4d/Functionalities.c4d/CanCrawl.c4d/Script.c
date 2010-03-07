/*-- Kriechen --*/
//(Code teilweise aus dem Westernpack.)

#strict

/* Einstellungen */

static const CRAWL_AIM_Max = 50;
local crosshair;


/* Allgemeines */

// Kriecht gerade
public func IsCrawling() { return(WildcardMatch(GetAction(), "*Crawl*")); }

public func StartCrawling()
{
  //Hinlegen
  if((GetProcedure() eq "WALK") ||
     (GetProcedure() eq "DIG") ||
     (GetProcedure() eq "BRIDGE") ||
     (GetProcedure() eq "BUILD") ||
     (GetProcedure() eq "PUSH") ||
     (GetProcedure() eq "CHOP") ||
     (GetProcedure() eq "LIFT"))
    return(SetAction("StartCrawl"));
    
  if(this()->~IsAiming())
  {
    this()->~StopAiming();
    return(SetAction("StartCrawl"));
  }
  
  return(false);
}

public func StopCrawling()
{
  //Aufstehen
  if(!IsCrawling() || !CanStandUp2()) return(false);
  SetXDir();
  return(SetAction("FlatUp")); 
}

/*public func ContextStopCrawl()
{
  [$CtxStopCrawl$|Image=MCMC|Condition=IsCrawling]
  StopCrawling();
  return(1);
}*/

public func Ready2Crawl()
{
  return(!GetEffect("NoCrawl") && !IsCrawling() && (GetAction() eq "WalkArmed" || GetAction() eq "Walk"));
}

/*public func ContextStartCrawl()
{
  [$CtxStartCrawl$|Image=MCMC|Condition=Ready2Crawl]
  StartCrawling();
}*/

public func CanStandUp()
{
  //Aufstehen durch Material behindert?
  return(!GBackSolid(0,-7-5));
}

public func CanStandUp2()
{
  //Aufstehen auch möglich, wenn über dem Clonk grabbares Material ist.
  if(GBackSolid(0,-7-5))
    if(!GetMaterialVal("DigFree","Material",GetMaterial(0,-7-5)))
      return(false);
  
  return(true);
}

/*protected func Scaling()
{
  _inherited();
  
  var dir = GetDir()*2-1;
  if(!GBackSolid(6*dir,0) && !CanStandUp())
  {
    SetAction("Crawl");
    SetPosition(GetX()+(dir*10),GetY()-10);
    SetXDir(dir*20);
  }
}*/

//Callbacks der Aktionen...
protected func StartCrawl()
{
  //this()->~CheckArmed();

  // Wenn nötig Effekt erzeugen 
  if(!GetEffect("Crawl", this())) AddEffect("Crawl", this(), 1, 0, this());
}

protected func CheckCrawlFall()
{
  if(CanStandUp())
    SetAction("FlatUp"); 
  else
  {
    if(Abs(GetXDir()) < 2)
    {
      var x;
      if(!GBackSolid(-4,4-5))
        x--;
      if(!GBackSolid(+4,4-5))
        x++;
        
      if(x)
      {
        SetXDir(x*10);
      }
    }
  }
}

protected func AbortCrawl() 
{
  var act = GetAction();

  // Bei manchen Aktionen nicht abbrechen   
  if(act eq "Scale")  return(SetAction("Crawl")); // Anstoßen an der Wand
  if(act eq "Hangle") return(SetAction("Crawl")); // Anstoßen an der Decke
  if(act eq "Tumble") return(SetAction("Crawl")); // Bei Objekttreffern liegen bleiben
  if(act eq "Walk")   return(SetAction("Crawl")); // Mysteriöse Walk-Aktion
  
  if((act ne "AimCrawl") && WildcardMatch(act, "Aim*"))
  {
    //var phase = GetPhase();
    SetAction("AimCrawl");
    //SetPhase(phase);
    return(1);
  }
  
  if(act eq "Jump")
  {
    if(!CanStandUp())
      return(SetAction("CrawlFall"));
    else
    {
      RemoveEffect("Crawl", this());

      if(GetDir() == DIR_Left)
        SetDir(DIR_Right);
      else
        SetDir(DIR_Left);
      
      SetComDir(COMD_Stop);
        
      SetXDir();
      AddEffect("IntCrawl2Scale",this(),10,1,this());
      
      var i = 10;
      var dir = -(GetDir()*2-1);
      SetPosition(GetX()+dir,GetY()+8+5);
      while(i-- > 0)
      {
        if(!Stuck())
        {
          SetPosition(GetX()-dir,GetY());
          break;
        }
        SetPosition(GetX()+dir,GetY());
      }
      
      //SetPosition(GetX()+(GetDir()*2-1)*-4,GetY()+8+5);
      SetAction("Scale");
      return(1);
    }
  }
  
  if(IsCrawling())  return(0);
  // Shape und Vertices zurücksetzen
  RemoveEffect("Crawl", this());
  SetAction("FlatUp"); 
}

private func UpdateVertices()
{
  if(GetAction() eq "AimCrawl") return();//Ignorieren.

  var x,y,r;
  this->~WeaponAt(x,y,r);
  SetVertex(0,0,x/1000 * (GetDir()*2-1),0,2);
  SetVertex(0,1,y/1000,0,2);
}

private func ResetVertices()
{
  if(GetAction() eq "AimCrawl") return();//Ignorieren.

  SetVertex(0,0,0);
  SetVertex(0,1,0);
}

public func FxIntCrawl2ScaleTimer()
{
  SetXDir();
  return(-1);
}

protected func UpdateTransferZone()
{
  if(IsCrawling()) 
  { 
    RemoveEffect("Crawl", this());
    AddEffect("Crawl", this(), 1, 0, this());
  }
  return(_inherited());
}


/* Kriech-Effekt */
public func FxNoCrawlTimer() { return(-1); }

public func FxCrawlStart(pClonk, iNum)
{
  // Physical, Shape und Vertixes anpassen
  EffectVar(0, pClonk, iNum) = GetPhysical("Walk", 0);
  SetPhysical("Walk", GetPhysical("Walk", 0)/4, 2);
  SetShape(-8, 2-5, 16, 8);
  SetVertexXY(0, 0,5-5);
  SetVertexXY(1, 0,2-5);
  SetVertexXY(2, 0,9-5);
  SetVertexXY(3,-2,3-5);
  SetVertexXY(4, 2,3-5);
  SetVertexXY(5,-4,3-5);
  SetVertexXY(6, 4,3-5);
  SetPosition(GetX(),GetY()+5);
}

public func FxCrawlStop(pClonk, iNum)
{
  // Physical, Shape und Vertices zurücksetzen
  SetPhysical("Walk", EffectVar(0, pClonk, iNum), 2);
  SetShape(-8, -10, 16, 20);
  SetVertexXY(0, 0, 0);
  SetVertexXY(1, 0,-7);
  SetVertexXY(2, 0, 9);
  SetVertexXY(3,-2,-3);
  SetVertexXY(4, 2,-3);
  SetVertexXY(5,-4, 3);
  SetVertexXY(6, 4, 3);
  SetPosition(GetX(),GetY()-5);//ugh
}


/* Overloads */

protected func ControlThrow()
{
  if(IsCrawling() && this()->~IsArmed() && !this()->~ReadyToFire() && this()->~ReadyToAim())
  {
    this()->~StartAiming();
    return(1);
  }
  return(_inherited(...));
}

public func ControlUp()
{
  if(IsCrawling() && !this()->~IsAiming())
  {
    StopCrawling();
    return(1);
  }
  return(_inherited(...));
}

public func ControlDig()
{
  if(GetPlrDownDouble(GetOwner()) && !IsCrawling() && Ready2Crawl() && !Contained())
  {
    StartCrawling();
    return(true);
  }
  return(_inherited(...));
}

public func ControlDigSingle()
{
  //Beim Kriechen kann man nicht graben
  if(IsCrawling()) return(1);
  return(_inherited(...));
}

/*protected func ControlDown()
{
  if(Contents(0))
    if(GetPlrDownDouble(GetOwner()) &&
      (Contents(0)->~CanAim()) && !this()->~IsAiming() &&
      !GetEffect("SquatAimTimeout") &&
      (GetAction() eq "Crawl"))
      {
         StartSquatAiming(); 
         return(1);
      }
  return(_inherited());
}*/

public func ReadyToAim()
{
   var val = _inherited();
   if(val) return(val);
   if(/*GetAction() eq "CrawlArmed" || */GetAction() eq "Crawl") return(1);
}

private func CheckArmed()
{
  //if (GetAction() eq "Crawl") if (this()->~IsArmed2()) return(SetAction("CrawlArmed"));
  //if (GetAction() eq "CrawlArmed") if (!this()->~IsArmed2()) return(SetAction("Crawl"));
  if (GetAction() eq "AimCrawl") if (!this()->~IsArmed2()) return(SetAction("Crawl"));
  return(_inherited());
}

public func WeaponAt(&x, &y, &r)
{
  if(this()->~IsAiming() && (GetAction() eq "AimCrawl"))
  {
    r = (Abs(crosshair->GetAngle())-90);
    x = 8000;
    y = 7000-5000;
    return(1);
  }
  
  return(_inherited(x,y,r));

  /*if(_inherited(x,y,r)) return(1);

  if(GetAction() eq "CrawlArmed")
  {
    x = 8000;
    y = 7000;
    return(1);
  }

  return(0);*/
}

public func DoAiming(int iChange)
{
  if(IsCrawling())
  {
    var angle = Abs(crosshair->GetAngle()) + iChange;
    if(!Inside(angle,90-CRAWL_AIM_Max,90+CRAWL_AIM_Max) && !Inside(angle,270-CRAWL_AIM_Max,270+CRAWL_AIM_Max))
      return;
  }

  return _inherited(iChange);
}

public func DoMouseAiming(int iTx, int iTy)
{
  if(IsCrawling())
  {
    var angle = Normalize(Angle(GetX(),GetY(),iTx,iTy),-180);
    if(!Inside(angle,90-CRAWL_AIM_Max,90+CRAWL_AIM_Max) && !Inside(angle,270-CRAWL_AIM_Max,270+CRAWL_AIM_Max))
      return;
  }
  
	return _inherited(iTx,iTy);
}

public func StopAiming()
{
  if(!IsCrawling()) return(_inherited());
  var val = _inherited();
  if(val)
    SetAction("Crawl");
  return(val);
}