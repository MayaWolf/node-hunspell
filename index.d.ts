declare module "hunspell" {
    export default class Hunspell {
        add(word: string): Promise<number>
        addDictionary(path: string): Promise<number>
        addWithAffix(word: string, example: string): Promise<number>
        analyze(word: string): Promise<ReadonlyArray<string>>
        generate(word: string, word2: string): Promise<ReadonlyArray<string>>
        generate(word: string, morphDescription: ReadonlyArray<string>): Promise<ReadonlyArray<string>>
        remove(word: string): Promise<number>
        spell(word: string): Promise<boolean>
        stem(word: string): Promise<ReadonlyArray<string>>
        stem(morph: ReadonlyArray<string>): Promise<ReadonlyArray<string>>
        suggest(word: string): Promise<ReadonlyArray<string>>
        suffixSuggest(rootWord: string): Promise<ReadonlyArray<string>>

        addSync(word: string): number
        addDictionarySync(path: string): number
        addWithAffixSync(word: string, example: string): number
        analyzeSync(word: string): ReadonlyArray<string>
        generateSync(word: string, word2: string): ReadonlyArray<string>
        generateSync(word: string, morphDescription: ReadonlyArray<string>): ReadonlyArray<string>
        removeSync(word: string): number
        spellSync(word: string): boolean
        stemSync(word: string): ReadonlyArray<string>
        stemSync(morph: ReadonlyArray<string>): ReadonlyArray<string>
        suggestSync(word: string): ReadonlyArray<string>
        suffixSuggestSync(rootWord: string): ReadonlyArray<string>
    }
}