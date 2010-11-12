/*-- Fallschirm --*/

#strict 2

//Einklinken und Ausklinken
public func ControlDigDouble(pObj)
{
  Set(pObj);
}

public func Set(object pObj)
{
  Sound("ParachuteOpen*.ogg");
  SetAction("Open", pObj);
}

protected func Fly()
{
  if(WildcardMatch(GetAction(),"*Free*"))
  {
    SetXDir(GetWind(GetX(), GetY()) / 8);
    SetYDir(30, 0, 10);
    if(GetContact(0,-1, CNAT_Bottom) || InLiquid())
    {
      SetAction("FoldFree");
      Sound("ParachuteClose.ogg");
    }
  }
  else
  {
    var targ = GetActionTarget();
    
    //Windbeeinflussung
    SetXDir(GetWind(GetX(), GetY()) /8 , targ);

    //Fall verlangsamen
    SetYDir(30,targ,10);

    if(GetPlrDownDouble(GetController(targ)))
      SetAction("StartFlyFree");
    
    if(GetContact(0, -1, CNAT_Bottom) || GetContact(targ, -1, CNAT_Bottom) || InLiquid() || InLiquid(targ))
      Close();
  }
}

/* Aktionen */

protected func Opening()
{
  if(GetActTime() > 25)
  {
    SetObjDrawTransform(1000, 0, 0, 0, 1000, 0, this);
    SetAction("Fly", GetActionTarget());
    return;
  }
  var w = Sin(36 * GetActTime() / 10, 1000);
  var h = 40 * GetActTime();

  SetObjDrawTransform(w, 0, 0, 0, h, InvertA1(500 * GetObjHeight() * h / 1000, 1000 * GetObjHeight()) - 500 * GetObjHeight(), this);
}

public func Close()
{
  //Zusammenfallen
  SetAction("Fold", GetActionTarget());
  Sound("ParachuteClose.ogg");
}

private func Folded()
{
  //Verschwinden
  FadeOut();
}