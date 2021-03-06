#include "Source\NeuralNet\DenseLayer.hpp"
#include "Source\Math\Random.hpp"


DenseLayer::DenseLayer(Mat::VectorSize const & layerSize, Activation activation)
	: mLayerSize(layerSize), mActivation(activation)
{
}







Mat::Vector<NetNodeType> DenseLayer::apply(Mat::Vector<NetNodeType> const & in) const
{
	return mActivation(mMatrix * in + mBias);
}


Mat::Vector<NetNodeType>&& DenseLayer::apply(Mat::Vector<NetNodeType> && in) const
{
	return std::move(mActivation(mMatrix * in + mBias));
}




void DenseLayer::mutate()
{
	//Determine mutation chances
	float chanceOfMutationForMatrix = Random::getRandomFloatBetween(0.f, 0.04f);
	float chanceOfMutationForBias = Random::getRandomFloatBetween(0.f, 0.04f);
	float constexpr mutationStrengthConstant = 0.003f;

	//Mutate matrix
	for (unsigned int x = 0; x < mMatrix.getSize().x(); ++x)
	{
		for (unsigned int y = 0; y < mMatrix.getSize().y(); ++y)
		{
			float mutationDice = Random::getRandomFloatBetween(0.f, 1.f);
			if (mutationDice > 1.f - chanceOfMutationForMatrix)
			{
				float mutationStrength = Random::getRandomFloatBetween(-mutationStrengthConstant, mutationStrengthConstant);
				mMatrix.at(Mat::XY(x, y)) += mutationStrength;
			}
		}
	}

	//Mutate bias
	for (unsigned int i = 0; i < mBias.getSize(); ++i)
	{
		float mutationDice = Random::getRandomFloatBetween(0.f, 1.f);
		if (mutationDice > 1.f - chanceOfMutationForBias)
		{
			float mutationStrength = Random::getRandomFloatBetween(-mutationStrengthConstant, mutationStrengthConstant);
			mBias.at(i) += mutationStrength;
		}
	}
}



void DenseLayer::setEntriesRandom()
{
	//Randomize matrix
	for (unsigned int x = 0; x < mMatrix.getSize().x(); ++x)
	{
		for (unsigned int y = 0; y < mMatrix.getSize().y(); ++y)
		{
			mMatrix.at(Mat::XY(x, y)) = Random::getRandomFloatBetween(-1.f, 1.f);
		}
	}

	//Randomize bias
	for (unsigned int i = 0; i < mBias.getSize(); ++i)
	{
		mBias.at(i) = Random::getRandomFloatBetween(-1.f, 1.f);
	}
}




Mat::Matrix<NetNodeType> const & DenseLayer::getMatrix() const
{
	return mMatrix;
}
void DenseLayer::setMatrix(Mat::Matrix<NetNodeType> const & matrix)
{
	mMatrix = matrix;
}
void DenseLayer::setMatrix(Mat::Matrix<NetNodeType> && matrix)
{
	mMatrix = std::move(matrix);
}



Mat::Vector<NetNodeType> const & DenseLayer::getBias() const
{
	return mBias;
}
void DenseLayer::setBias(Mat::Vector<NetNodeType> const & vector)
{
	mBias = vector;
}
void DenseLayer::setBias(Mat::Vector<NetNodeType> && vector)
{
	mBias = std::move(vector);
}



Activation DenseLayer::getActivation() const
{
	return mActivation;
}

void DenseLayer::setActivation(Activation activation)
{
	mActivation = activation;
}



Mat::VectorSize DenseLayer::getLayerSize() const
{
	return mLayerSize;
}

