// $Id$

#ifndef SAMPLEPLAYER_HH
#define SAMPLEPLAYER_HH

#include "SoundDevice.hh"
#include "Resample.hh"
#include "shared_ptr.hh"
#include <vector>

namespace openmsx {

class MSXMotherBoard;
class WavData;

class SamplePlayer : public SoundDevice, private Resample
{
public:
	SamplePlayer(MSXMotherBoard& motherBoard, const std::string& name,
	             const std::string& desc, const XMLElement& config,
	             const std::string& samplesBaseName, unsigned numSamples);
	~SamplePlayer();

	void reset();

	/** Start playing a (new) sample.
	 */
	void play(unsigned sampleNum);

	/** Keep on repeating the given sample data.
	 * If there is already a sample playing, that sample is still
	 * finished. If there was no sample playing, the given sample
	 * immediatly starts playing.
	 * Parameters are the same as for the play() method.
	 * @see stopRepeat()
	 */
	void repeat(unsigned sampleNum);

	/** Stop repeat mode.
	 * The currently playing sample will still be finished, but won't
	 * be started.
	 * @see repeat()
	 */
	void stopRepeat();

	/** Is there currently playing a sample. */
	bool isPlaying() const;

private:
	inline int getSample(unsigned index);
	void doRepeat();

	// SoundDevice
	virtual void setOutputRate(unsigned sampleRate);
	virtual void generateChannels(int** bufs, unsigned num);
	virtual bool updateBuffer(unsigned length, int* buffer,
		const EmuTime& time, const EmuDuration& sampDur);

	// Resample
	virtual bool generateInput(int* buffer, unsigned num);

	std::vector<shared_ptr<WavData> > samples; // change to unique_ptr in the future

	const void* sampBuf;
	unsigned inFreq;
	unsigned outFreq;
	unsigned index;
	unsigned bufferSize;
	unsigned currentSampleNum;
	unsigned nextSampleNum;
	bool bits8;
};

} // namespace openmsx

#endif
