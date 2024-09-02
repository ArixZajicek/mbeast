from enum import Enum, auto

class Standard(Enum):
    Neutral = auto()
    Happy = auto()
    Smirk = auto()
    owo = auto()
    Embarrassed = auto()
    uwu = auto()
    Disappointed = auto()
    Sad = auto()
    Angry = auto()
    xwx = auto()
    Confused = auto()
    Sleepy = auto()
    Dizzy = auto()

standard_expr: Standard = Standard.Neutral

running = True