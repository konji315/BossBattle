//`/`/`//`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/
//Content:ƒVƒ“ƒOƒ‹ƒgƒ“ƒNƒ‰ƒX‚Ìƒeƒ“ƒvƒŒ[ƒg
//
//day:2017/07/19
//
//acotr:Koshida Kenji
//`/`/`//`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/`/
#pragma once

template<class T>
class singleton
{
public:
    static inline T& GetInstance()
    {
        static T instance;
        return instance;
    }

protected:
    singleton() {}
    virtual ~singleton() {}

private:
    void operator=(const singleton& obj) {} // ‘ã“ü‰‰Zq‹Ö~
    singleton(const singleton &obj) {}      // ƒRƒs[ƒRƒ“ƒXƒgƒ‰ƒNƒ^‹Ö~
};