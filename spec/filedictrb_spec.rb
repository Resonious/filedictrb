# frozen_string_literal: true

RSpec.describe Filedict do
  before do
    Dir.glob('tmp/*').each do |tmpfile|
      FileUtils.rm tmpfile unless File.directory?(tmpfile)
    end
  end

  it 'has a version number' do
    expect(Filedict::VERSION).not_to be nil
  end

  it 'lets me open a hash' do
    hash = Filedict::Hash.new('tmp/test.fdict')
    expect(hash).to be_a Filedict::Hash
  end

  it 'lets me read an empty hash' do
    hash = Filedict::Hash.new('tmp/test.fdict')
    expect(hash['my test key']).to be_a Filedict::Set
  end

  it 'lets me write then read' do
    hash = Filedict::Hash.new('tmp/test.fdict')
    hash['test key'].add 'test value'
    expect(hash['test key']).to include 'test value'
  end

  it 'lets me read from another instance' do
    hash1 = Filedict::Hash.new('tmp/test.fdict')
    hash2 = Filedict::Hash.new('tmp/test.fdict')

    hash1['test key'].add 'test value!'
    hash1['test key'].add 'yet another'
    expect(hash2['test key']).to eq Filedict::Set['test value!', 'yet another']
  end
end
