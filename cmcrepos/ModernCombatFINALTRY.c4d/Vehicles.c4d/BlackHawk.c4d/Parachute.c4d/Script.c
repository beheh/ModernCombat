/*-- Fallschirm --*/

#strict


/* Initialisierung */

protected func Initialize()
{
  SetGraphics("Fade");
  return(1);
}

//Einklinken und Ausklinken
public func ControlDigDouble(pObj)
{
  Set(pObj);
}

public func Set(object pObj)
{
  Sound("Click");
  SetAction("Open",pObj);
}

private func Fly()
{
  //Windbeeinflussung
  SetXDir(GetWind(GetX(),GetY())/8,GetAttObj());

  //Fall verlangsamen
  var speed = GetYDir(GetAttObj(),10);
  SetYDir(30,GetAttObj(),10);

  if(GetPlrDownDouble(GetController(GetAttObj())) || (Abs(speed) < 2))
    Close();
}

public func GetAttObj()
{
  return(GetActionTarget());
}

/* Aktionen */

private func Opening()
{
  if(GetActTime() > 25)
  {
    SetObjDrawTransform(1000,0,0,0,1000,0,this());
    SetAction("Fly",GetActionTarget());
    return();
  }
  var w = Sin(90*(1000*GetActTime()/(25))/1000,1000);
  var h = 1000*GetActTime()/(25);

  SetObjDrawTransform(w,0,0,0,h,(500*GetObjHeight())-((1000*GetObjHeight())*h/1000),this());
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