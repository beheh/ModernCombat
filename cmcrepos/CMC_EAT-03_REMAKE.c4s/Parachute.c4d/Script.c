/*-- Fallschirm --*/
#strict

protected func Initialize()
{
  if(GetOwner() == NO_OWNER) SetColorDw(RGBa(0,0,0,255));
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
  Attach(pObj,0,0);
  SetAction("Open",pObj);
  SetActionData(256 + iVertex);
}

//Fall abbremsen
private func Fly()
{
  SetXDir(GetWind(GetX(),GetY())/8,GetAttObj());
  
	var speed = GetYDir(GetAttObj(),10);
	if(speed>=18)
		SetYDir(speed/11,GetAttObj(),10);
    
  if(GetPlrDownDouble(GetController(GetAttObj())) || (Abs(speed) < 2))
    Close();
}

public func GetAttObj()
{
  return(GetActionTarget());
}

//Effekte
private func Opening()//auf
{
  if(GetActTime() > 35)
  {
    SetObjDrawTransform(1000,0,0,0,1000,0,this());
    SetAction("Fly",GetActionTarget());
    return();
  }
  var w = Sin(90*(1000*GetActTime()/(35))/1000,1000);
  var h = 1000*GetActTime()/(35);

  SetObjDrawTransform(w,0,0,0,h,(500*GetObjHeight())-((1000*GetObjHeight())*h/1000),this());
}

public func Close()
{
  Detach();
  SetAction("Fold");
}

private func Folded()
{
  FadeOut4K(10);
}

//Alt!
private func Folding()//zu
{
  if(GetActTime() > 35)
  {
    RemoveObject();
    return();
  }
  var w = 1001-(Sin(90*(1000*GetActTime()/(35))/1000,1000));
  var h = 1001-(1000*GetActTime()/(35));

  SetObjDrawTransform(w,0,0,0,h,(500*GetObjHeight())-((1000*GetObjHeight())*h/1000),this());
}


/* Attach-Sachen */
local iVertex, pAttach;
private func Attach(object pTarget, int iX, int iY)
{
  Detach();
  
  pAttach = pTarget;
  SetPosition(GetX(pAttach),GetY(pAttach));
  
  AddVertex(iX,iY,pAttach);
  iVertex = GetVertexNum(pAttach)-1;
  SetVertex(iVertex,0,iX,pAttach,1);
  SetVertex(iVertex,1,iY,pAttach,1);
}

private func Detach()
{
  if(!pAttach) return();
  RemoveVertex(iVertex,pAttach);

  if(GetProcedure() eq "ATTACH")
    SetAction("Fold");
}

public func AttachTargetLost()
{
  Detach();
}