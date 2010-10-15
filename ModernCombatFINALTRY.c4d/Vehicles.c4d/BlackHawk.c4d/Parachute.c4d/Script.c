/*-- Fallschirm --*/

#strict 2


/* Initialisierung */

protected func Initialize()
{
  return 1;
}

//Einklinken und Ausklinken
public func ControlDigDouble(pObj)
{
  Set(pObj);
}

public func Set(object pObj)
{
  Sound("ParachuteOpen*.ogg");
  SetAction("Open",pObj);
}

private func Fly()
{
  if(WildcardMatch(GetAction(),"*Free*"))
  {
    SetXDir(GetWind(GetX(),GetY())/8);
    SetYDir(30,0,10);
    if(GetContact(0,-1,CNAT_Bottom))
    {
      SetAction("FoldFree");
      Sound("ParachuteClose.ogg");
    }
  }
  else
  {
    var targ = GetActionTarget();
    
    //Windbeeinflussung
    SetXDir(GetWind(GetX(),GetY())/8,targ);

    //Fall verlangsamen
    SetYDir(30,targ,10);

    if(GetPlrDownDouble(GetController(targ)))
      SetAction("StartFlyFree");
    
    if(GetContact(0,-1,CNAT_Bottom) || GetContact(targ,-1,CNAT_Bottom))
      Close();
  }
}

/* Aktionen */

private func Opening()
{
  if(GetActTime() > 25)
  {
    SetObjDrawTransform(1000,0,0,0,1000,0,this());
    SetAction("Fly",GetActionTarget());
    return;
  }
  var w = Sin(90*(1000*GetActTime()/(25))/1000,1000);
  var h = 1000*GetActTime()/(25);

  SetObjDrawTransform(w,0,0,0,h,InvertA1(500*GetObjHeight()*h/1000,1000*GetObjHeight())-500*GetObjHeight(),this());
}

public func Close()
{
  //Zusammenfallen
  SetAction("Fold",GetActionTarget());
  Sound("ParachuteClose.ogg");
}

private func Folded()
{
  //Verschwinden
  FadeOut();
}
