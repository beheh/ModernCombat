/*-- Dunkelheit --*/

//Dunkelheit beeinflusst nicht die Färbung des Himmels.

#strict
#appendto DARK


global func SetDarkness(int iGrade)
{
  var obj;
  if(GetID(this()) != DARK)
    obj = FindObject(DARK);
  else
    obj = this();
  //kein Dunkelheit-Objekt: Keine Dunkelheit
  if(!obj)
    return(false);

  iGrade = BoundBy(iGrade,0,10);

  obj->LocalN("darkness") = iGrade*100;
  var g = BoundBy(128-iGrade*10,0,128);
  SetGamma(RGB(0,0,0),RGB(g,g,g),RGB(255,255,255),2);

  obj->UpdateLights();

  return(true);
}

func FxFadingTimer(object pTarget, int iEffectNumber, int iEffectTime)
{
  var grade = EffectVar(1,pTarget,iEffectNumber);
  var end = EffectVar(0,pTarget,iEffectNumber);
  var fade = EffectVar(2,pTarget,iEffectNumber);
  var go = EffectVar(3,pTarget,iEffectNumber);
  if(grade == end)
    return(-1);

  go += fade;

  if(go >= 10)
  {
    if(grade > end)
    {
      grade = Max(grade-go/10, end);
    }
    else
    {
      grade = Min(grade+go/10, end);
    }
    go = 0;

    EffectVar(1,pTarget,iEffectNumber) = grade;

    var g = BoundBy(128-grade*100/1000,0,128);

    SetGamma(RGB(0,0,0),RGB(g,g,g),RGB(255,255,255),2);

    pTarget->LocalN("darkness") = grade;

    UpdateLights();
  }

  EffectVar(3,pTarget,iEffectNumber) = go;
}