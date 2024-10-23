# Procedure to add syscall

## files to make changes to:

- syscall.h
- syscall.c
- sysproc.c
- usys.S
- user.h
- Makefile

## 1 - edit syscall.h
```c
#define SYS_getyear 22
```

## 2 - edit syscall.c
```c
extern int sys_getyear(void);

[SYS_getyear] sys_getyear
```

## 3 - edit sysproc.c
```c
int sys_getyear(void)
{
    return 1975;
}
```

## edit usys.S
```c
SYSCALL(getyear)
```

## edit user.h
```c
int getyear(void);
```

## making c file to test getyear

```bash
vim test_get_year.c
```

```c
#include "types.h"
#include "stat.h"
#include "user.h"

int main(void)
{
    printf(1, "Unix start time: %d", getyear());
    exit();
}
```

```Makefile
UPROGS=
    _test_get_year\


EXTRA=
    test_get_year.c\
    README
```