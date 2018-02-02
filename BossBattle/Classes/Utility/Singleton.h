//�`/�`/�`//�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/
//Content:�V���O���g���N���X�̃e���v���[�g
//
//day:2017/07/19
//
//acotr:Koshida Kenji
//�`/�`/�`//�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/�`/
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
    void operator=(const singleton& obj) {} // ������Z�q�֎~
    singleton(const singleton &obj) {}      // �R�s�[�R���X�g���N�^�֎~
};