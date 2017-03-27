#include <stdio.h>
#include <stdlib.h>
#include <string>

using namespace std;

int UTF8ToUnicode(const char *pmbs, wchar_t *pwcs, int size)
{
    int cnt = 0;
    // ���� size-- ��Ԥ�ȳ�ȥβ������λ��
    if (pmbs != NULL && pwcs != NULL && size-- > 0) {
        while (*pmbs != 0 && size > 0) {
            unsigned char ch = *pmbs;
            if (ch > 0x7FU) {
                int cwch = 0;
                while (ch & 0x80U) {
                    ch <<= 1;
                    cwch++;
                }
                *pwcs = *pmbs++ & (0xFFU >> cwch);
                while (--cwch > 0) {
                    *pwcs <<= 6;
                    *pwcs |= (*pmbs++ & 0x3FU);
                }
            } else {
                *pwcs = *pmbs++;
            }
            pwcs++;
            size--;
            cnt++;
        }
        *pwcs = 0;
        cnt++;
    }
    return cnt;
}

int UnicodeToUTF8(const wchar_t *pwcs, char *pmbs, int size)
{
    int cnt = 0;
    // ���� size-- ��Ԥ�ȳ�ȥβ������λ��
    if (pwcs != NULL && pmbs != NULL && size-- > 0) {
        while (*pwcs != 0 && size > 0) {
            if (*pwcs < 0x00000080U) {
                *pmbs++ = (char)*pwcs;
                size -= 1;
                cnt += 1;
            } else if (*pwcs < 0x00000800U) {
                // ʣ��ռ䲻����Ÿ��ַ�
                if (size < 2) {
                    break;
                }
                *pmbs++ = (0xFFU << 6) | (*pwcs >> 6);
                *pmbs++ = 0x80U | (*pwcs & 0x3FU);
                size -= 2;
                cnt += 2;
            } else if (*pwcs < 0x00010000U) {
                // ʣ��ռ䲻����Ÿ��ַ�
                if (size < 3) {
                    break;
                }
                *pmbs++ = (0xFFU << 5) | (*pwcs >> 12);
                *pmbs++ = 0x80U | ((*pwcs >> 6) & 0x3FU);
                *pmbs++ = 0x80U | (*pwcs & 0x3FU);
                size -= 3;
                cnt += 3;
            } else if (*pwcs < 0x00200000U) {
                // ʣ��ռ䲻����Ÿ��ַ�
                if (size < 4) {
                    break;
                }
                *pmbs++ = (0xFFU << 4) | (*pwcs >> 18);
                *pmbs++ = 0x80U | ((*pwcs >> 12) & 0x3FU);
                *pmbs++ = 0x80U | ((*pwcs >> 6) & 0x3FU);
                *pmbs++ = 0x80U | (*pwcs & 0x3FU);
                size -= 4;
                cnt += 4;
            } else if (*pwcs < 0x04000000U) {
                // ʣ��ռ䲻����Ÿ��ַ�
                if (size < 5) {
                    break;
                }
                *pmbs++ = (0xFFU << 3) | (*pwcs >> 24);
                *pmbs++ = 0x80U | ((*pwcs >> 18) & 0x3FU);
                *pmbs++ = 0x80U | ((*pwcs >> 12) & 0x3FU);
                *pmbs++ = 0x80U | ((*pwcs >> 6) & 0x3FU);
                *pmbs++ = 0x80U | (*pwcs & 0x3FU);
                size -= 5;
                cnt += 5;
            } else if (*pwcs < 0x80000000U) {
                // ʣ��ռ䲻����Ÿ��ַ�
                if (size < 6) {
                    break;
                }
                *pmbs++ = (0xFFU << 2) | (*pwcs >> 30);
                *pmbs++ = 0x80U | ((*pwcs >> 24) & 0x3FU);
                *pmbs++ = 0x80U | ((*pwcs >> 18) & 0x3FU);
                *pmbs++ = 0x80U | ((*pwcs >> 12) & 0x3FU);
                *pmbs++ = 0x80U | ((*pwcs >> 6) & 0x3FU);
                *pmbs++ = 0x80U | (*pwcs & 0x3FU);
                size -= 6;
                cnt += 6;
            } else {
                // �޷�ʶ��� Unicode �ַ�
                break;
            }
            pwcs++;
        }
        *pmbs = 0;
        cnt++;
    }
    return cnt;
}

int main(void)
{
    // �ⲿ�ִ������� Linux �ϲ��ԣ�Windows �� char ���ͷ� UTF-8 ���롣
    char mbs2[] = "��������123";
    wchar_t wcs2[256] = { 0 };
    int ret2 = UTF8ToUnicode(mbs2, wcs2, sizeof(wcs2)/sizeof(wchar_t));
    printf("%d\n", ret2);
    wstring uni_str(wcs2);
    int out_size = uni_str.size();
    printf("%d\n", out_size);
    
    // �ⲿ�ִ������� Linux �ϲ��ԣ�Windows �������޷���ӡ UTF-8 �ַ�����
    char mbs1[256] = { 0 };
    //wchar_t wcs1[] = L"��������";
    int ret1 = UnicodeToUTF8(wcs2, mbs1, sizeof(mbs1)/sizeof(char));
    printf("%d\n", ret1);
    printf("%s\n", mbs1);
    return 0;
}
