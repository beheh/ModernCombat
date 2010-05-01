/*-- Geschoss --*/

#strict

func Initialize() {
   return(1);
}

func ResetRotation() {
    SetR(Angle(GetX(),GetY(),GetX()+GetXDir(),GetY()+GetYDir()),this());
}

func Hit()
{
    Explode(30+RandomX(0,10));
}
