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
    Explode(40+RandomX(0,20));
}
