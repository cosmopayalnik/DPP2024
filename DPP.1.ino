#define BIG 1000
#define RND 15
#define MNUM 10


void key_generator(long *k, long *d, long *s) {
    long p = random_simple();
    long q = random_simple();
    while (p == q) {
        q = random_simple();
    }
    long sl = p * q;
    long f = (p - 1) * (q - 1);
    long kl = mutual_simple(f);
    long dl = search_d(f, kl);
    *k = kl;
    *d = dl;
    *s = sl;
}

long random_simple() {
    randomSeed(micros());
    long num = random() % RND + 2;
    while (true) {
        if (is_simple(num)) {
            break;
        } else {
            num++;
        }
    }
    return num;
}


bool is_simple(long x) {
    bool result = true;
    for (long i = 2; i < x; i++) {
        if (x % i == 0) {
            result = false;
            break;
        }
    }
    return result;
}


long mutual_simple(long x) {
    long res = x - 1;
    while (res > 0) {
        long n = nod(x, res);
        if (n == 1) {
            break;
        } else {
            res--;
        }
    }
    return res;
}


long nod(long a, long b) {
  if (b == 0) {
    return a;
    } else {
    return nod(b, a % b);
    }
}


long search_d(long f, long k) {
    long d = long(f / k) + k;
    while (true) {
        if ((d * k) % f == 1) {
            break;
        } else {
            d++;
        }
    }
    return d;
}


long encrypter(long k, long s, long m) {
    char mpk[BIG];
    long_to_char(m, mpk);
    power_big(mpk, k);
    long res = mod_big(mpk, s);
    return res;
}


long decrypter(long d, long s, long b) {
    char bpd[BIG];
    long_to_char(b, bpd);
    power_big(bpd, d);
    long res = mod_big(bpd, s);
    return res;
}


void long_to_char(long num, char *bignum) {
    for (int i = 0; i < BIG; i++) {
        bignum[i] = '0';
    }
    int idx = BIG - 1;
    while (num > 0) {
        long cif = num % 10;
        bignum[idx] = cif + 48;
        num = num / 10;
        idx--;
    }
}


long char_to_long(char *bignum) {
    long res = 0;
    for (int i = BIG - 1; i >= 0; i--) {
        long temp = bignum[i] - 48;
        if (i < BIG - 1) {
            for (int j = 0; j < BIG - i - 1; j++) {
                temp = temp * 10;
            }
        }
        res = res + temp;
    }
    return res;
}


void sum_big(char *n1, char *n2, char *s) {
    int rank = 0;
    for (int i = BIG - 1; i >= 0; i--) {
        int digit1 = n1[i] - 48;
        int digit2 = n2[i] - 48;
        s[i] = (digit1 + digit2 + rank) % 10 + 48;
        rank = (digit1 + digit2 + rank) / 10;
    }
}


void mult_big(char *m1, long m2) {
    if (m2 > 1) {
        char mul_num[BIG], temp_num[BIG];
        for (int i = 0; i < BIG; i++) {
            mul_num[i] = m1[i];
        }
        for (long j = 1; j < m2; j++) {
            sum_big(m1, mul_num, temp_num);
            for (int k = 0; k < BIG; k++) {
                m1[k] = temp_num[k];
            }
        }
    }
}


void power_big(char *p1, long p2) {
    long mnog = char_to_long(p1);
    if (p2 > 1) {
        for (long i = 1; i < p2; i++) {
            mult_big(p1, mnog);
        }
    }
}


bool is_bigger(char *num1, char *num2) {
    bool res = true;
    for (int i = 0; i < BIG; i++) {
        if (num1[i] > num2[i]) {
            break;
        } else if (num1[i] < num2[i]){
            res = false;
            break;
        }
    }
    return res;
}


void sub_big(char *um, char *vch, char *res) {
    int rank = 0;
    for (int i = BIG - 1; i >= 0; i--) {
        int num1 = um[i] - 48;
        int num2 = vch[i] - 48;
        if (num1 == 0 && rank == -1) {
            num1 = 9;
            rank = -1;
            res[i] = num1 - num2 + 48;
        } else {
            num1 = num1 + rank;
            if (num1 >= num2) {
                res[i] = num1 - num2 + 48;
                rank = 0;
            } else {
                num1 = num1 + 10;
                res[i] = num1 - num2 + 48;
                rank = -1;
            }
        }
    }
}


long mod_big(char *bignum, long delit) {
    long res = 0;
    char del[BIG], lost[BIG];
    long_to_char(delit, del);
    booster(bignum, del);
    if (is_bigger(bignum, del)) {
        for (int i = 0; i < BIG; i++) {
            lost[i] = bignum[i];
        }
        while (is_bigger(lost, del)) {
            sub_big(bignum, del, lost);
            for (int i = 0; i < BIG; i++) {
                bignum[i] = lost[i];
            }
        }
        res = char_to_long(lost);
    } else {
        res = char_to_long(bignum);
    }
    return res;
}


void booster(char *num, char *del) {
    int num_start = -1, del_start = -1;
    for (int i = 0; i < BIG; i++) {
        if (num[i] > 48 && num_start == -1) {
            num_start = i;
        }
        if (del[i] > 48 && del_start == -1) {
            del_start = i;
        }
    }
    if (num_start < del_start - 1) {
        char temp[BIG], temp2[BIG];
        for (int i = 0; i < BIG; i++) {
            temp[i] = '0';
        }
        int pos = num_start + 1;
        for (int i = del_start; i < BIG; i++) {
            temp[pos] = del[i];
            pos++;
        }
        sub_big(num, temp, temp2);
        for (int i = 0; i < BIG; i++) {
            num[i] = temp2[i];
        }
        booster(num, del);
    }
}

void setup() {
  Serial.begin(115200);
  long k = 0, d = 0, s = 0;
  key_generator(&k, &d, &s);
  Serial.print("public key is: ");
  Serial.print(k);
  Serial.print(" ");
  Serial.println(s);
  long c = encrypter(k, s, MNUM);
  Serial.print("Encrypted M is: ");
  Serial.println(c);
  long b = decrypter(d, s, c);
  Serial.print("decrypted: ");
  Serial.println(b);
}

void loop() {}
