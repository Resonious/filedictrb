# frozen_string_literal: true

RSpec.describe Filedict do
  it 'has a version number' do
    expect(Filedict::VERSION).not_to be nil
  end

  it 'does something useful' do
    hash = Filedict::Hash.new('test.fdict')
    expect(hash).to be_a Filedict::Hash
  end
end
