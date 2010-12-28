#strict 2
#appendto LASR

public func Set(int iAngle, int iWidth, int iDist, int iT, object pDmg, object pAtt, int iReflect, bool fNoStuckRemove)
{
  // optional parameters
  if(!iWidth) iWidth = 3;
  if(!iDist) iDist = 300;
  // iT = 0: no out-fading
  iTime = iT;
	reflections = iReflect; // how often the Ray should be reflected

  // reset fade-timer
  c = 0;
  // new attach and damage callback objects (optional)
  pDmgCallback = pDmg;
  pAttach = pAtt;
  if(pAttach)
  {
    dx = GetX()-GetX(pAttach);
    dy = GetY()-GetY(pAttach);
  }

  // create light
  pLight = CreateObject(LALI, 0, 0, -1);

  SetMaxDistance(iDist);
  SetAngle(iAngle);
  SetWidth(iWidth);
  HitCheck();
  DrawTransform();

  // we are stuck?
  if(Stuck() && !fNoStuckRemove)
    RemoveObject();
}