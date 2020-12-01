# python3
import sys
import numpy as np

'''
Solve the Profile HMM Problem.
     Input: A threshold θ, followed by an alphabet Σ, followed by a multiple alignment
     Alignment whose strings are formed from Σ.
     Output: The transition matrix followed by the emission matrix of HMM(Alignment, θ).

Note: Your matrices can be either space-separated or tab-separated.

Sample Input:
0.289
--------
A B C D E
--------
EBA
E-D
EB-
EED
EBD
EBE
E-D
E-D
Sample Output:
	S	I0	M1	D1	I1	M2	D2	I2	E	
S	0	0	1.0	0	0	0	0	0	0
I0	0	0	0	0	0	0	0	0	0
M1	0	0	0	0	0.625	0.375	0	0	0
D1	0	0	0	0	0	0	0	0	0
I1	0	0	0	0	0	0.8	0.2	0	0
M2	0	0	0	0	0	0	0	0	1.0
D2	0	0	0	0	0	0	0	0	1.0
I2	0	0	0	0	0	0	0	0	0
E	0	0	0	0	0	0	0	0	0
--------
	A	B	C	D	E
S	0	0	0	0	0
I0	0	0	0	0	0
M1	0	0	0	0	1.0
D1	0	0	0	0	0
I1	0	0.8	0	0	0.2
M2	0.143	0	0	0.714	0.143
D2	0	0	0	0	0
I2	0	0	0	0	0
E	0	0	0	0	0
'''

class ProfileHMM:

  # theta;
  # alphabet;
  # alignment;

    def __init__(self, theta, alphabet, alignment):
        # theta, alphabet, alignment = self.readFromFile()
        self.theta = theta;
        self.alphabet = alphabet;
        self.alignment = alignment;
        transition, emission = self.profile(theta, alphabet, alignment)
        states = self.getAllStates(transition.shape[0])
        fullTransition = self.getFullTransition(transition)
        self.saveTransitionAndEmission(alphabet, states, fullTransition, emission)

    def readFromFile(self):
        f = open('input.txt', 'r')
        data = f.read().split()
        theta = float(data[0])
        ind = [i for i in range(len(data)) if '--------' == data[i]]
        alphabet = data[ind[0]+1:ind[1]]
        alignment = np.array([[*s] for s in data[ind[1]+1:]])
        f.close()
        return theta, alphabet, alignment

    def profile(self, theta, alphabet, alignment):
        alphabetDict = ({alphabet[i]:i for i in range(len(alphabet))}, {i:alphabet[i] for i in range(len(alphabet))})
        n, l = alignment.shape
        threshold = theta * n
        kept = [True] * l
        for i in range(l):
            if sum('-' == alignment[:, i]) >= threshold:
                kept[i] = False
        levels = [0 for _ in range(l)]
        for i in range(l):
            levels[i] = levels[i-1]
            if kept[i]:
                levels[i] += 1

        def getIndex(level, state):
            if 0 == level:
                if 0 == state:
                    return 1
                else:
                    return 0
            if 0 != state:
                return 3*level-2+state
            else:
                return 3*level+1

        transition = np.zeros((sum(kept)*3+3, 3), dtype = float)
        # 0: I; 1: M; 2: D/E
        emission = np.zeros((sum(kept)*3+3, len(alphabet)), dtype = float)

        for i in range(n):
            lastLevel = 0
            lastState = -1
            lastInd = getIndex(lastLevel, lastState)
            for j in range(l):
                currLevel = levels[j]
                if kept[j]:
                    currState = 2 if '-' == alignment[i, j] else 1
                    currInd = getIndex(currLevel, currState)
                    transition[lastInd, currState] += 1
                    if 1 == currState:
                        emission[currInd, alphabetDict[0][alignment[i, j]]] += 1
                    lastInd = currInd
                else:
                    if '-' != alignment[i, j]:
                        currState = 0
                        currInd = getIndex(currLevel, currState)
                        transition[lastInd, currState] += 1
                        emission[currInd, alphabetDict[0][alignment[i, j]]] += 1
                        lastInd = currInd
            transition[lastInd, 2] += 1
        
        for i in range(transition.shape[0]):
            sum1 = sum(transition[i, :])
            if 0 != sum1:
                transition[i, :] /= sum1
            sum2 = sum(emission[i, :])
            if 0 != sum2:
                emission[i, :] /= sum2
        
        return transition, emission

    def getAllStates(self, n):
        # n: number of states in total
        states = ['' for _ in range(n)]
        states[0] = 'S'
        states[-1] = 'E'
        states[1] = 'I0'
        s = ('M', 'D', 'I')
        for i in range(2, n-1):
            states[i] = s[(i+1)%3] + str((i+1)//3)
        return states

    def getFullTransition(self, transition):
        fullTransition = np.zeros((transition.shape[0], transition.shape[0]), dtype = float)
        fullTransition[0, 1:4] = transition[0, :]

        for i in range(1, transition.shape[0]-4):
            if 1 == i%3:
                fullTransition[i, i:i+3] = transition[i, :]
            elif 2 == i%3:
                fullTransition[i, i+2:i+5] = transition[i, :]
            else:
                fullTransition[i, i+1:i+4] = transition[i, :]
        for i in range(-4, -1):
            fullTransition[i, -2] = transition[i, 0]
            fullTransition[i, -1] = transition[i, -1]

        return fullTransition

    def saveTransitionAndEmission(self, alphabet, states, fullTransition, emission):
        f = open('result.txt', 'w')
        print(' '.join([' '] + states))
        f.write('\t'+'\t'.join(states) + '\n')
        for i in range(fullTransition.shape[0]):
            print(' '.join([states[i]] + list(map(str, fullTransition[i, :]))))
            f.write('\t'.join([states[i]] + list(map(str, fullTransition[i, :])))+'\n')
        print('--------')
        f.write('--------'+'\n')
        print(' '.join([' '] + alphabet))
        f.write('\t'+'\t'.join(alphabet)+'\n')
        for i in range(emission.shape[0]):
            print(' '.join([states[i]] + list(map(str, emission[i, :]))))
            f.write('\t'.join([states[i]] + list(map(str, emission[i, :])))+'\n')
        f.close()

if __name__ == "__main__":
    theta = float(sys.stdin.readline().strip())
    sys.stdin.readline()  # delimiter

    sigma = sys.stdin.readline().strip().split()
    sys.stdin.readline()  # delimiter

    alignment =  np.array([[line.strip()] for line in sys.stdin])


    alignment = np.array([])
    while True:
      inner_array = np.arr([])
        inner_array = inner_array.append(line.strip() for line in sys.stdin)
    
    phmm = ProfileHMM(theta, sigma, alignment)

    # print(profile_hmm(theta, sigma, alignment))
