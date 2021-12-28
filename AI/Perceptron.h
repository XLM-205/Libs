/*-------Perceptron Base-------
*	??
*	--??
*
*
*	Moon Wiz Studios (c) - 10/08/2020
*	by: Ramon Darwich de Menezes
*
*
*	YOU MAY NOT use this file for commercial purposes without permission from this file creator OR Moon Wiz Studios
*	YOU MAY use it in any project of your own or edit this file, given the proper credits to Moon Wiz Studios
*   This notice MAY NOT be removed nor altered from any source distribution
*
*	Version 0.0.0
*/

#ifndef _H_MWPERCEPTRON_
#define _H_MWPERCEPTRON_

#include <stdio.h>									//Used for 'DataSet' file reading and MLP import and exporting
#include <stdlib.h>									//Used for 'rand()' method
#include <math.h>									//
#include <omp.h>									//For multi-threaded neuron evaluation

//#define MLP_DEBUG
//#define MLP_VERBOSE								//Outputs more text in some functions
#define RAND_METHOD (rand() / (double)RAND_MAX)		//Random number generation method. Used for making values between [0..1]. Overwrite this for whatever method required
#define SAFE_DELETE(tgt)	if(tgt != nullptr) { delete tgt; tgt = nullptr; }
#define SAFE_DELETE_ARR(tgt) if(tgt != nullptr) { delete[] tgt; tgt = nullptr; }

//Holds a set of 'i' lines of data containing 'j' values each, the last value being the exepected value
class TrainingDataSet
{
private:
	bool m_isSetReady;		//If true, the set is ready to be used (Read from file and populated). If false, well, we don't recomend using it
	int m_valuesPerLine;	//How many 'j' values should we read per line?
	int m_entries;			//How many 'i' lines should we read?
	double **m_data;		//Holds all the read data. The last index of each row 'i' is reserved for the expected value

	void Clear(void)
	{
		if(m_data)
		{
			for(int i = 0; i < m_valuesPerLine; i++)
			{
				delete[] m_data[i];
				m_data[i] = nullptr;
			}
			delete[] m_data;
		}
	}

public:
	TrainingDataSet(int valuesPerLine, int lines) : m_valuesPerLine(valuesPerLine), m_entries(lines), m_data(nullptr)
	{
		changeTrainingSize(m_valuesPerLine, m_entries);
	}
	TrainingDataSet(int valuesPerLine, int lines, const char *trainingSet) : m_valuesPerLine(valuesPerLine), m_entries(lines), m_data(nullptr)
	{
		changeTrainingSize(m_valuesPerLine, m_entries);
		m_isSetReady = changeTrainingSet(trainingSet);
	}

	~TrainingDataSet()
	{
		Clear();
	}

	void changeTrainingSize(int valuesPerLine, int lines)
	{
		Clear();
		m_entries = lines;
		m_valuesPerLine = valuesPerLine;
		m_data = new double*[m_entries];
		for(int i = 0; i < m_entries; i++)
		{
			m_data[i] = new double[m_valuesPerLine]();
		}
	}

	//Changes the training set from a file. Returns false if it was unsuccessful 
	bool changeTrainingSet(const char *trainingSet)
	{
		FILE *fp = fopen(trainingSet, "r");
		if(fp)
		{
			int stop = m_valuesPerLine - 1;
			for(int i = 0; i < m_entries && (fgetc(fp) != EOF); i++)
			{
				fseek(fp, -1L, SEEK_CUR);
				for(int j = 0; j < stop; j++)
				{
					(void)fscanf(fp, "%lf", &m_data[i][j]);		//'(void)' so the compiler don't emit more warnings
				}
				(void)fscanf(fp, "%lf%*c", &m_data[i][stop]);
			}
			fclose(fp);
			m_isSetReady = true;
			return m_isSetReady;
		}
#ifdef MLP_VERBOSE
		printf("[TDTSET][ERROR] Failed to read training set @ '%s' !\n", trainingSet);
#endif
		m_isSetReady = false;
		return m_isSetReady;
	}

	void changeTraining(int valuesPerLine, int lines, const char *trainingSet)
	{
		changeTrainingSize(valuesPerLine, lines);
		changeTrainingSet(trainingSet);
	}

	int getAmountLines(void)
	{
		return m_entries;
	}
	int getAmountValuesPerLine(void)
	{
		return m_valuesPerLine;
	}
	double getValue(int lineIndex, int valueIndex)
	{
		return m_data[lineIndex][valueIndex];
	}
	double getValueExpectedOfLine(int lineIndex)
	{
		return m_data[lineIndex][m_valuesPerLine - 1];
	}

	bool isDataSetReady(void)
	{
		return m_isSetReady;
	}

	void fillInputStream(double *inStream, int lineIndex)
	{
		for(int i = 0; i < m_valuesPerLine; i++)
		{
			inStream[i] = m_data[lineIndex][i];
		}
	}
};

class NeuronBase;

//Creates a neuron with n sinapses initialized with input: 1 and weight 0
class NeuronBase
{
protected:
	const int  m_qInputs;	//Amount of inputs this neuron have. Add +1 to account for the bias sinapse
	double m_bias;			//Bias Weight
	double m_output;		//Output value
	double *m_weights;		//Weights for the inputs
	double **m_input;		//Input addresses
	double m_loc_grad;		//Local Gradient of the neuron

	//Function that should determines the final m_output of the neuron
	virtual double Activation(double sum) {	return sum;	}	

	void init_weights(double ratio)
	{
		//TODO Why on XOR this doesn't work
		//double ratio = 1.0 / (m_qInputs + 1);
		//double ratio = 1.0;
		m_bias = RAND_METHOD * ratio;
		for(int i = 0; i < m_qInputs; i++)
		{
			m_weights[i] = RAND_METHOD * ratio;
		}
	}
	
public:
	NeuronBase(int inputs) : m_qInputs(inputs), m_output(0.0), m_loc_grad(0.0)
	{ 
		m_weights = new double[m_qInputs]();
		m_input = new double*[m_qInputs]();
		init_weights(1.0);
	}
	~NeuronBase()
	{
		SAFE_DELETE_ARR(m_input)
		SAFE_DELETE_ARR(m_weights)
	}

	//Returns the d/dx of the activation function. Class derivations of this should change this as well. If the activation function doesn't have a derivative, return 0
	virtual double Derivative(void)		{ return 0;	}
	virtual double Derivative(double x)	{ return 0;	}
	////Updates all the weights of the neuron using the formula: Wi = Wi + LR * error * Xi
	//virtual void updateWeightAll(double LearningRate, double error)
	//{
	//	for(int i = 0, stop = getSinapseAmount(); i < stop; i++)
	//	{
	//		m_sinapse[i].setWeight(m_sinapse[i].getWeight() + LearningRate * error * m_sinapse[i].getInput());
	//	}
	//}
	//The result of this neuron
	virtual double Result(void)
	{
		m_output = Activation(getSum());
		return m_output;
	}
	//Compares the output with an expected value, allowing to clamp it. !! The neuron must have been evaluated by a previous 'Result()' call !!
	virtual double ComputeError(double expected)
	{
		return expected - m_output;
	}

	void setInput(double *inp, double weight, int inputIndex)
	{
		m_input[inputIndex] = inp;
		m_weights[inputIndex] = weight;
	}
	void setInputAddress(double *inp, int inputIndex)
	{
		m_input[inputIndex] = inp;
	}
	void setInputWeight(double weight, int inputIndex)
	{
		m_weights[inputIndex] = weight;
	}
	void updateInputWeight(double weight, int inputIndex)	//Add over the current weight
	{
		m_weights[inputIndex] += weight;
	}
	void updateBias(double weight)							//Add over the current weight
	{
		m_bias += weight;
	}
	void setBias(double weight)
	{ 
		m_bias = weight;
	}
	void setLocalGradient(double grad)
	{
		m_loc_grad = grad;
	}

	double getBias(void)
	{ 
		return m_bias;
	}
	const double* getInputAddress(int inputIndex)
	{
		return m_input[inputIndex];
	}
	double getInputWeight(int inputIndex)
	{
		return m_weights[inputIndex];
	}
	double getInputValue(int inputIndex)
	{
		return *m_input[inputIndex];
	}
	int getInputAmount(void)
	{
		return m_qInputs;
	}
	
	//Evaluates the neuron
	double getSum(void)
	{
		double sum = m_bias;
		for (int i = 0; i < m_qInputs; i++)
		{
			sum += *m_input[i] * m_weights[i];
		}
		return sum;
	}
	double getOutput(void)
	{
		return m_output;
	}
	const double* getOutputAddress(void)
	{
		return &m_output;
	}
	//Resets all the weights on the neuron, including bias and if true rationed by the amount of inputs
	void resetWeights(bool distributeWeights)
	{
		if(distributeWeights)
		{
			init_weights(1.0 / (m_qInputs + 1));
		}
		else
		{
			init_weights(1.0);
		}
	}
	/*double* getOutputAddress(void)
	{
		return &m_output;
	}*/
	double getLocalGradient(void)
	{
		return m_loc_grad;
	}

	bool isInputSet(int index)
	{
		return m_input[index] != nullptr;
	}
	void linkOutputAsNeuronInput(NeuronBase *neuron, int inputIndex)
	{
		//Setting 'inputTarget' to points to this neuron's output value and making sure
		if(neuron != nullptr)	
		{
			neuron->m_input[inputIndex] = &m_output;
		}
	}
	bool matchesWithOutput(const double *address)
	{
		return address == &m_output;
	}

	void Print(void)
	{
		printf("\tBIAS= %.4lf\n", m_bias);
		for(int i = 0; i < m_qInputs; i++)
		{
			printf("\tX%3d= %.4lf 0x%p\t w%3d= %.4lf\t->\t%.4lf\n", i, *m_input[i], m_input[i], i, m_weights[i], *m_input[i] * m_weights[i]);
		}
		printf("\tOut = %.4lf 0x%p\n", m_output, &m_output);
	}
};

// Creates a layer of neurons
template<class NeuronType>
class NeuronLayer
{
private: 
	bool m_isReady = false;				//Becomes true if the layer is ready to be used
	int m_neuMax;						//Amount of m_neurons this layer contains
	NeuronType **m_neurons = nullptr;	//Assumes polymorphism and will rely solely on 'NeuronBase' methods

	void buildLayer(int inputEach)
	{
		m_neurons = new NeuronType*[m_neuMax];
		for (int i = 0; i < m_neuMax; i++)
		{
			m_neurons[i] = new NeuronType(inputEach);
		}
	}

public:
	NeuronLayer() : m_neuMax(0) { }
	NeuronLayer(int neuronAmount, int inputEach) : m_neuMax(neuronAmount)
	{
		buildLayer(inputEach);
	}
	~NeuronLayer()
	{
		if(m_neurons != nullptr)
		{
			for (int i = 0; i < m_neuMax; i++)
			{
				delete m_neurons[i];
				m_neurons[i] = nullptr;
			}
			delete[] m_neurons;
			m_neurons = nullptr;
		}
	}

	//Links each output FROM THIS LAYER to the target layer input. Target layer MUST have one input for each output, excluding the bias
	bool connectLayers(NeuronLayer *targetLayer)
	{
		if (targetLayer == nullptr  || m_neurons == nullptr || m_neuMax != (targetLayer->m_neurons[0]->getInputAmount()))
		{
			//Failed to link: Either the target targetLayer or this targetLayer doesn't exist or it's input doesn't matches the amount of m_neurons of this targetLayer
			m_isReady = false;
			return m_isReady;
		}
		for (int i = 0; i < targetLayer->m_neuMax; i++)
		{
			for (int j = 0; j < m_neuMax; j++)
			{
				//From each neuron 'j', link it's output Address to each input 'j' for each neuron 'i' of the next targetLayer
				m_neurons[j]->linkOutputAsNeuronInput(targetLayer->m_neurons[i], j);
			}
		}
		m_isReady = true;
		return m_isReady;
	}

	NeuronType* getNeuron(int index)
	{
		return m_neurons[index];
	}

	void buildLayer(int neuronAmount, int inputEach, bool isInputOutputLayer)
	{
		if (m_neurons == nullptr)	//Only build the layer if it wasn't built yet
		{
			m_isReady = isInputOutputLayer;
			m_neuMax = neuronAmount;
			buildLayer(inputEach);
		}
	}

	bool isReady(void)
	{
		return m_isReady;
	}

	int getNeuronAmount(void)
	{
		return m_neuMax;
	}

	void setInput(int neuronIndex, int inputIndex, double *inputAddress, double weight)
	{
		m_neurons[neuronIndex]->setInput(inputAddress, weight, inputIndex);
	}
	void setBias(int neuronIndex, double weight)
	{
		m_neurons[neuronIndex]->setBias(weight);
	}

	//Executes the layer, without returning a value (since we cannot assume this layer have multiple neurons)
	void Execute(void)
	{
		//#pragma omp parallel for
		for(int i = 0; i < m_neuMax; i++)
		{
			m_neurons[i]->Result();
		}
	}
	//Return the value of Output neuron at 'outputNeuronIndex'
	double Result(int outputNeuronIndex)
	{ 
		return m_neurons[outputNeuronIndex]->getOutput();
	}

	void Print(void)
	{
		if (m_neurons)
		{
			for (int i = 0; i < m_neuMax; i++)
			{
				printf("Neuron %d 0x%p (%3d inputs) input Addresses:\n", i + 1, m_neurons[i], m_neurons[i]->getInputAmount());
				printf("\tBias: %8.4lf\n", m_neurons[i]->getBias());
				for (int j = 0; j < m_neurons[i]->getInputAmount(); j++)
				{
					if(m_neurons[i]->isInputSet(j))
					{
						printf("\tX%3d from 0x%p\n", j, m_neurons[i]->getInputAddress(j));
						printf("\t\tInp: %8.4lf\tx\tw: %8.4lf\t=\t%8.4lf\n", m_neurons[i]->getInputValue(j), m_neurons[i]->getInputWeight(j), m_neurons[i]->Result());
					}
					else
					{
						printf("\t\tInp: NULL\tx\tw: %8.4lf\t=\t[!! WARNING INPUT NOT SET !!]\n", m_neurons[i]->getInputWeight(j));
					}
				}
				printf("\tOutput= %8.4lf \t Loc.Gradient: %8.4lf \t 0x%p\n", m_neurons[i]->getOutput(), m_neurons[i]->getLocalGradient(), m_neurons[i]->getOutputAddress());
			}
		}
		else
		{
			printf("[N.LYR][ERROR] Layer not built!\n");
		}
	}
};

template<class NeuronType>
class MultilayerNetwork
{
private:
	int m_hiddenLayers;							//How many hidden layers exists within the network
	int m_outputLayer;							//Index of the output layer. Shortcut for 'hiddenLayers + 1', because we always have an input and output layers
	int m_amountLayers;							//Total amount of layers
	NeuronLayer<NeuronType> *m_MLN = nullptr;	//Multi Layer Network array. Index [0] is the input layer and [m_outputLayer]

	int verifyLayerRecursive(int curLayer)
	{
		if(curLayer == 0)
		{
			return 0;
		}
		for(int i = 0; i < m_MLN[curLayer].getNeuronAmount(); i++)
		{
			NeuronType *neu = m_MLN[curLayer].getNeuron(i);
			//If 'neu' is nullptr, something went wrong
			if(neu == nullptr)
			{
				return 1;
			}
			//Making sure curLayer - 1 output's are curLayer's inputs
			for(int j = 0; j < neu->getInputAmount(); j++)
			{
				NeuronType *tester = m_MLN[curLayer - 1].getNeuron(i);
				if(!tester->matchesWithOutput(neu->getInputAddress(j)))
				{
					return 0;	//This input is not pointing to the back's output!
				}
			}
			return verifyLayerRecursive(curLayer - 1);
		}
		return -1;	//Shouldn't reach here. If we did, this layer have no neurons!
	}

	void PurgeNetwork(void)
	{
		if(m_MLN)
		{
			delete[] m_MLN;
			m_MLN = nullptr;
		}
		m_hiddenLayers = m_outputLayer = m_amountLayers = 0;
	}

public:
	MultilayerNetwork(const char *MLPWeightFile)
	{
		ImportWeights(MLPWeightFile);
	}
	MultilayerNetwork(int hiddenLayers, int inpNeuronAmount, int inputAmount) : m_hiddenLayers(hiddenLayers < 0 ? 0 : hiddenLayers), m_outputLayer(m_hiddenLayers + 1), m_amountLayers(m_hiddenLayers + 2)
	{
		m_MLN = new NeuronLayer<NeuronType>[2 + hiddenLayers]();
		m_MLN[0].buildLayer(inpNeuronAmount, inputAmount, true);
	}

	~MultilayerNetwork()
	{
		PurgeNetwork();
	}

	bool ImportWeights(const char *MLPWeightsFile)
	{
		PurgeNetwork();
		FILE *fp = fopen(MLPWeightsFile, "r");
		if(fp)
		{
			int inputAmount = 0, inpNeuronAmount = 0, i = 0;
			if(fscanf(fp, "%d %d\n%d\n", &inputAmount, &m_hiddenLayers, &inpNeuronAmount) == 3)	//Must read 3 values
			{
				m_hiddenLayers = m_hiddenLayers < 0 ? 0 : m_hiddenLayers;
				m_outputLayer = m_hiddenLayers + 1;
				m_amountLayers = m_hiddenLayers + 2;

				//All good, allocate layers and stuff
				m_MLN = new NeuronLayer<NeuronType>[2 + m_hiddenLayers]();
				m_MLN[0].buildLayer(inpNeuronAmount, inputAmount, true);
				do 
				{
					for(int j = 0; j < m_MLN[i].getNeuronAmount(); j++)
					{
						NeuronBase *neu = m_MLN[i].getNeuron(j);
						int inps = neu->getInputAmount();
						double weight = 0.0;
						if(fscanf(fp, "%lf", &weight) != 1)		//Reading bias
						{
							PurgeNetwork();
#ifdef MLP_VERBOSE
							printf("[MLP][ERROR] Couldn't import weight data from '%s' !\n > File is damaged or bad formated", MLPWeightsFile);
#endif							
							fclose(fp);
							return false;
						}
						neu->setBias(weight);
						for(int k = 0; k < inps; k++)
						{
							if(fscanf(fp, " %lf", &weight) != 1)	//Reading weights
							{
								PurgeNetwork();
#ifdef MLP_VERBOSE
								printf("[MLP][ERROR] Couldn't import weight data from '%s' !\n > File is damaged or bad formated", MLPWeightsFile);
#endif							
								fclose(fp);
								return false;
							}
							neu->setInputWeight(weight, k);
						}
					}
					if(i == m_amountLayers - 1)
					{
#ifdef MLP_VERBOSE
						printf("[MLP][INFO] Weight data successufully imported!\n");
#endif					
						fclose(fp);
						return true;
					}
					else if(fscanf(fp, "%d\n", &inpNeuronAmount) != 1)
					{
						PurgeNetwork();
#ifdef MLP_VERBOSE
						printf("[MLP][ERROR] Couldn't import weight data from '%s' !\n > File is damaged or bad formated", MLPWeightsFile);
#endif					
						fclose(fp);
						return false;
					}
					setLayer(i + 1, inpNeuronAmount);
				} while(++i < m_amountLayers);
			}
		}
		PurgeNetwork();
#ifdef MLP_VERBOSE
		printf("[MLP][ERROR] Couldn't import weight data from '%s' !\n > File couldn't be opened", MLPWeightsFile);
#endif
		return false;
	}
	void ExportWeights(const char *OutputPath)
	{
		FILE *fp = fopen(OutputPath, "w");
		fprintf(fp, "%d %d\n", m_MLN[0].getNeuron(0)->getInputAmount(), m_hiddenLayers);
		for(int i = 0; i < m_amountLayers; i++)
		{
			fprintf(fp, "%d\n", m_MLN[i].getNeuronAmount());
			for(int j = 0; j < m_MLN[i].getNeuronAmount(); j++)
			{
				NeuronBase *neu = m_MLN[i].getNeuron(j);
				fprintf(fp, "%lf", neu->getBias());
				for(int k = 0; k < neu->getInputAmount(); k++)
				{
					fprintf(fp, " %lf", neu->getInputWeight(k));
				}
				fprintf(fp, "\n");
			}
		}
		fclose(fp);
	}

	//Custom set the 'weight' of conection 'inputIndex' from neuron 'neuronIndex' located on layer 'layerIndex'
	void setWeight(int layerIndex, int neuronIndex, int inputIndex, double weight)
	{
		m_MLN[layerIndex].getNeuron(neuronIndex)->setInputWeight(weight, inputIndex);
	}
	void setBias(int layerIndex, int neuronIndex, double weight)
	{
		m_MLN[layerIndex].getNeuron(neuronIndex)->setBias(weight);
	}
	void setBias(double weight)
	{
		for(int i = 0; i < m_amountLayers; i++)
		{
			for(int j = 0; j < m_MLN[i].getNeuronAmount(); j++)
			{
				m_MLN[i].getNeuron(j)->setBias(weight);
			}
		}
	}
	void setInputStream(int neuronIndex, int inputIndex, double *inputAddress)
	{
		m_MLN[0].getNeuron(neuronIndex)->setInputAddress(inputAddress, inputIndex);
	}
	//Fills all the input neurons with a 1:1 correspondence to the input stream addresses. The 'addressArray' should match with the amount of inputs per neuron
	void setInputStreamAllNeurons(double *addressArray)
	{
		for(int i = 0; i < m_MLN[0].getNeuronAmount(); i++)
		{
			NeuronBase *neu = m_MLN[0].getNeuron(i);
			for(int j = 0; j < neu->getInputAmount(); j++)
			{
				neu->setInputAddress(&addressArray[j], j);
			}
		}
	}
	void setInputStreamAllNeurons(int inputIndex, double *inputAddress)
	{
		for(int i = 0; i < m_MLN[0].getNeuronAmount(); i++)
		{
			m_MLN[0].getNeuron(i)->setInputAddress(inputAddress, inputIndex);
		}
	}
	//Sets the 'weight' of connection 'inputIndex' from neuron 'neuronIndex' and sets the source of input 'inputAddress' of the hidden layer 0
	bool setInput(int neuronIndex, int inputIndex, double *inputAddress, double weight)
	{
		m_MLN[0].setInput(neuronIndex, inputIndex, inputAddress, weight);
		return true;
	}
	//Sets each hidden layer manually. If setting the last hidden layer, also sets the output layer
	bool setLayer(int hiddenIndex, int neuronAmount)
	{
		if (hiddenIndex > 0)
		{
			m_MLN[hiddenIndex].buildLayer(neuronAmount, m_MLN[hiddenIndex - 1].getNeuronAmount(), hiddenIndex == m_outputLayer);
			return m_MLN[hiddenIndex - 1].connectLayers(&m_MLN[hiddenIndex]);
		}
		//Invalid index
		return false;
	}
	//Sets the amount of output neurons we have at the last layer
	bool setOutputLayer(int neuronAmount)
	{
		return setLayer(m_outputLayer, neuronAmount);
	}

	int getLayerCount(void)
	{
		return m_amountLayers;
	}
	int getIndexOutput(void)
	{
		return m_outputLayer;
	}
	int getInputTotal(void)
	{
		int inpT = 0;
		for(int i = 0; i < m_amountLayers; i++)
		{
			for(int j = 0; j < m_MLN[i].getNeuronAmount(); j++)
			{
				inpT += m_MLN[i].getNeuron(j)->getInputAmount();
			}
		}
		return inpT;
	}
	NeuronType* getNeuron(int layerIndex, int neuronIndex)
	{
		return m_MLN[layerIndex].getNeuron(neuronIndex);
	}

	bool isNetworkReady(void)
	{
		int result = 0;
		for(int i = 0; i < m_amountLayers; i++)
		{
			if(!m_MLN[i].isReady())
			{
#ifdef MLP_VERBOSE
				printf("[MLP][WARNING] Layer %3d reports NOT being ready!\n", i);
#endif

				return false;
			}
			for(int j = 0; j < m_MLN[i].getNeuronAmount(); j++)
			{
				NeuronBase *neu = m_MLN[i].getNeuron(j);
				for(int k = 0; k < neu->getInputAmount(); k++)
				{
					if(!neu->isInputSet(k))
					{
#ifdef MLP_VERBOSE
						printf("[MLP][WARNING] Sinapse %4d from neuron %3d at layer %d3 input pointer is NULL!\n", k, j, i);
#endif
						return false;
					}
					if(neu->getInputWeight(k) == 0.0)
					{
#ifdef MLP_VERBOSE
						printf("[MLP][WARNING] Sinapse %4d from neuron %3d at layer %3d weight is 0.0! (Maybe uninitialized?)\n", k, j, i);
#endif
					}
				}
			}
		}
		//Verifies Back Propagation integrity
		//	* Goes from each output neuron to each input, fails if any connection is broken
		for(int i = 0; i < m_MLN[m_outputLayer].getNeuronAmount(); i++)
		{
			result += verifyLayerRecursive(m_amountLayers - 1);
		}
		if(result)
		{
#ifdef MLP_VERBOSE
			printf("[MLP][WARNING] Network reported NOT having a complete back-propagation path!\n");
#endif
			return false;
		}
#ifdef MLP_VERBOSE
		printf("[MLP][INFO] Network successfully passed all verification steps!\n");
#endif
		return true;
	}

	////Resets all the weights in the network, to the 'globalWeight' value, and set bias to 'bias'
	//void ResetWeights(double globalWeight, double bias)
	//{
	//	for (int i = 0; i < m_amountLayers; i++)
	//	{
	//		for (int j = 0; j < m_MLN[i].getNeuronAmount(); j++)
	//		{
	//			m_MLN[i].getNeuron(j)->setBias(bias);
	//			for (int k = 0; k < m_MLN[i].getNeuron(j)->getInputAmount(); k++)
	//			{
	//				m_MLN[i].getNeuron(j)->setInputWeight(globalWeight, k);
	//			}
	//		}
	//	}
	//}
	////Resets all the weights in the network, including bias, to the 'globalWeight' value
	//void ResetWeights(double globalWeight)
	//{
	//	ResetWeights(globalWeight, globalWeight);
	//}

#ifdef MLP_DEBUG
	NeuronLayer<NeuronType>* d_getLayer(int i)
	{
		return &m_MLN[i];
	}
#endif
	//Randomize all the weights on the network, between ['min'..'scale + min'] using standard rand()
	void RandomizeWeights(bool redistributeWeights)
	{
		NeuronBase *neuron = nullptr;
		for(int i = 0; i < m_amountLayers; i++)
		{
			for(int j = 0; j < m_MLN[i].getNeuronAmount(); j++)
			{
				m_MLN[i].getNeuron(j)->resetWeights(redistributeWeights);
			}
		}
	}
	//Picks a random neuron from a random layer and randomly changes it's weights 'shuffles' times
	//void Unstuck(double minW, double maxW, int shuffles)
	//{
	//	int layer = rand() % m_amountLayers;
	//	int neuron = rand() % m_MLN[layer].getNeuronAmount();
	//	m_MLN[layer].getNeuron(neuron)->setBias(RAND_METHOD * maxW + minW);
	//	for(int i = 0; i < shuffles; i++)
	//	{
	//		layer = rand() % m_amountLayers;
	//		neuron = rand() % m_MLN[layer].getNeuronAmount();
	//		m_MLN[layer].getNeuron(neuron)->setInputWeight(RAND_METHOD * maxW + minW, rand() % m_MLN[layer].getNeuron(neuron)->getInputAmount());
	//	}
	//}

	//Executes the whole network. Should be used only to yield results from the network
	void Execute(void)
	{
		for (int i = 0; i < m_amountLayers; i++)
		{
			m_MLN[i].Execute();
#ifdef MLP_VERBOSE
			printf("---------------------------------------------------------- [LAYER %3d EXECUTION]\n", i);
			Print();
#endif
		}
	}

	double TrainNetwork(double expected, double learningRate)
	{
		Execute();							//Execute the whole network
		NeuronBase *neuron = nullptr;		//Used to easily manipulate neurons
		//double ***varWeights = new double**[m_amountLayers]();	//[i] - Layer | [j] - Neuron | [k] - Weight AND bias
		double MSE = 0.0;					//Mean Square Error
		int lastLayer = m_amountLayers - 1;

		//varWeights[lastLayer] = new double*[m_MLN[m_outputLayer].getNeuronAmount()]();
		for(int i = 0; i < m_MLN[m_outputLayer].getNeuronAmount(); i++)
		{
			double e = 0.0;
			neuron = m_MLN[m_outputLayer].getNeuron(i);
			e = neuron->ComputeError(expected);
			//varWeights[lastLayer][i] = new double[neuron->getInputAmount() + 1]();
			neuron->setLocalGradient(e * neuron->Derivative());
			MSE += e * e;

			//Updating output Weights
			double lrLocGrad = learningRate * neuron->getLocalGradient();	//Optimization
			//varWeights[lastLayer][i][0] = lrLocGrad;
			neuron->updateBias(lrLocGrad);
			for(int j = 0; j < neuron->getInputAmount(); j++)
			{
				//neuron->setSinapseWeight(neuron->getSinapseWeight(j) - lrLocGrad * neuron->getSinapseInputValue(j), j);
				neuron->updateInputWeight(lrLocGrad * neuron->getInputValue(j), j);
				//varWeights[lastLayer][i][j + 1] = lrLocGrad * neuron->getInputValue(j);
			}
			//neuron->setBiasWeight(neuron->getBiasWeight() - lrLocGrad);
		}
		MSE = 0.5 * MSE;

		//Calculating hidden layers gradients
		for(int i = m_outputLayer - 1; i >= 0; i--)
		{
			//varWeights[i] = new double*[m_MLN[i].getNeuronAmount()]();
			for(int j = 0; j < m_MLN[i].getNeuronAmount(); j++)
			{
				double sumGrad = 0.0;			//Sum of the output gradients * their inputs
				int layerAhead = i + 1;			//Index of the layer ahead of current 'i'
				neuron = m_MLN[i].getNeuron(j);
				//varWeights[i][j] = new double[neuron->getInputAmount() + 1]();
				for(int k = 0; k < m_MLN[layerAhead].getNeuronAmount(); k++)
				{
					NeuronBase *neuronAhead = m_MLN[layerAhead].getNeuron(k);
					for(int l = 0; l < neuronAhead->getInputAmount(); l++)
					{
						sumGrad += neuronAhead->getLocalGradient() * neuronAhead->getInputWeight(k);
					}
				}
				neuron->setLocalGradient(sumGrad * neuron->Derivative());

				double lrLocGrad = learningRate * neuron->getLocalGradient();
				//varWeights[i][j][0] = lrLocGrad;
				neuron->updateBias(lrLocGrad);
				for(int k = 0; k < neuron->getInputAmount(); k++)
				{
					//varWeights[i][j][k + 1] = lrLocGrad * neuron->getInputValue(k);
					neuron->updateInputWeight(lrLocGrad * neuron->getInputValue(k), k);
					//neuron->setSinapseWeight(neuron->getSinapseWeight(k) - lrLocGrad * neuron->getSinapseInputValue(k), k);
				}
				//neuron->setBiasWeight(neuron->getBiasWeight() - lrLocGrad);
			}
		}
		//All done, now, update the weights of the whole network
		//for(int layer = 0; layer < m_amountLayers; layer++)
		//{
		//	for(int neu = 0; neu < m_MLN[layer].getNeuronAmount(); neu++)
		//	{
		//		neuron = m_MLN[layer].getNeuron(neu);
		//		neuron->updateBias(varWeights[layer][neu][0]);
		//		for(int input = 0; input < neuron->getInputAmount(); input++)	//Ofset to 1 to match the input weight array
		//		{
		//			neuron->updateInputWeight(varWeights[layer][neu][input+1], input);
		//		}
		//		if(varWeights[layer][neu] != nullptr)
		//		{
		//			delete[] varWeights[layer][neu];
		//			varWeights[layer][neu] = nullptr;
		//		}
		//		SAFE_DELETE_ARR(varWeights[layer][neu])
		//	}
		//	SAFE_DELETE_ARR(varWeights[layer])
		//}
		//SAFE_DELETE_ARR(varWeights)

		return MSE;
	}

	//Used to pull a given output from the network
	double Result(int outputNeuronIndex)
	{
		return m_MLN[m_outputLayer].Result(outputNeuronIndex);
	}

	

	void Print(void)
	{
		for (int i = 0; i < m_amountLayers; i++)
		{
			if (i == 0)
			{
				printf("[INPUT LAYER]------------------------");
			}
			else if (i == m_outputLayer)
			{
				printf("[OUTPUT LAYER]-----------------------");
			}
			else
			{
				printf("[LAYER %2d]--------------------------", i);
			}
			printf("[%s]\n", (m_MLN[i].isReady() ? "READY" : " !! NOT READY !! "));
			m_MLN[i].Print();
		}
	}

	void Print(int layerIndex)
	{
		m_MLN[layerIndex].Print();
	}

	void PrintWeights(int weightsPerLine, int layerIndex)
	{
		printf("[L%-3d] %s\n", layerIndex, (layerIndex == m_outputLayer ? "OUTPUT LAYER" : ""));
		for(int j = 0; j < m_MLN[layerIndex].getNeuronAmount(); j++)
		{
			NeuronBase *neu = m_MLN[layerIndex].getNeuron(j);
			printf("\t[N%-3d]\tbias: %11.8lf\tGrad: %11.8lf\tOut : %11.8lf\n\t", j, neu->getBias(), neu->getLocalGradient(), neu->getOutput());
			//printf("b(%d,%d)  : %11.6lf | grad: %11.6lf | LocGrad: %11.6lf\n", layerIndex, j, neu->getBiasWeight(), neu->getBiasGradient(), neu->getLocalGradient());
			for(int k = 0; k < neu->getInputAmount(); k++)
			{
				printf("\tW%-3d: %11.8lf", k, neu->getInputWeight(k));
				if(((k + 1) % weightsPerLine) == 0)
				{
					printf("\n\t");
				}
			}
			printf("\n");
		}
	}

	void PrintWeights(int weightsPerLine)
	{
		for(int i = 0; i < m_amountLayers; i++)
		{
			printf("[L%-3d] %s\n", i, (i == m_outputLayer ? "OUTPUT LAYER" : ""));
			for(int j = 0; j < m_MLN[i].getNeuronAmount(); j++)
			{
				NeuronBase *neu = m_MLN[i].getNeuron(j);
				printf("\t[N%-3d]\tbias: %11.8lf\tGrad: %11.8lf\tOut : %11.8lf\n\t", j, neu->getBias(), neu->getLocalGradient(), neu->getOutput());
				//printf("b(%d,%d)  : %11.6lf | grad: %11.6lf | LocGrad: %11.6lf\n", i, j, neu->getBiasWeight(), neu->getBiasGradient(), neu->getLocalGradient());
				for(int k = 0; k < neu->getInputAmount(); k++)
				{
					printf("\tW%-3d: %11.8lf", k, neu->getInputWeight(k));
					if(((k + 1) % weightsPerLine) == 0)
					{
						printf("\n\t");
					}
				}
				printf("\n");
			}
		}
	}

	void PrintWeightsSum(void)
	{
		for(int i = 0; i < m_amountLayers; i++)
		{
			double sumLayer = 0.0;
			for(int j = 0; j < m_MLN[i].getNeuronAmount(); j++)
			{
				NeuronBase *neuron = m_MLN[i].getNeuron(j);
				double sumW = neuron->getBias();
				for(int k = 0; k < neuron->getInputAmount(); k++)
				{
					sumW += neuron->getInputWeight(k);
				}
				sumLayer += sumW;
			}
			printf("[Layer: %3d (%3d neurons / %5d weights)] TOTw= %18.8lf\tAVGw= %11.8lf\n", i, m_MLN[i].getNeuronAmount(), m_MLN[i].getNeuron(0)->getInputAmount(), sumLayer, m_MLN[i].getNeuronAmount() / sumLayer);
		}
	}
};
#endif