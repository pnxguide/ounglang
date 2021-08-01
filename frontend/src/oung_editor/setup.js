import * as monaco from 'monaco-editor';

monaco.languages.register({ id: 'oung' });

monaco.languages.registerCompletionItemProvider('oung', {
  provideCompletionItems: (model, position, token) => {
    const suggestions = [
      {
        label: 'var',
        kind: monaco.languages.CompletionItemKind.Keyword,
        insertText: 'ouNG',
        documentation: 'byte variable',
      },
      {
        label: 'zero',
        kind: monaco.languages.CompletionItemKind.Snippet,
        insertText: 'oung',
        documentation: 'o? 0? ... what?',
      },
      {
        label: 'one',
        kind: monaco.languages.CompletionItemKind.Snippet,
        insertText: 'OUNG',
        documentation: '1 is 1',
      },
      {
        label: 'print',
        kind: monaco.languages.CompletionItemKind.Function,
        insertText: 'OunG $0 OUNg',
        insertTextRules:
          monaco.languages.CompletionItemInsertTextRule.InsertAsSnippet,
      },
      {
        label: 'semi',
        kind: monaco.languages.CompletionItemKind.Snippet,
        insertText: 'OUNg',
      },
      {
        label: 'str',
        kind: monaco.languages.CompletionItemKind.Snippet,
        insertText: 'Oung',
        documentation: 'create string',
      },
      {
        label: 'sep',
        kind: monaco.languages.CompletionItemKind.Keyword,
        insertText: 'OUng',
        documentation: 'separator',
      },
      {
        label: 'add',
        kind: monaco.languages.CompletionItemKind.Function,
        insertText: 'oUng ${1:var} OUng ${2:number} OUNg',
        insertTextRules:
          monaco.languages.CompletionItemInsertTextRule.InsertAsSnippet,
      },
      {
        label: 'sub',
        kind: monaco.languages.CompletionItemKind.Function,
        insertText: 'oUnG ${1:var} OUng ${2:number} OUNg',
        insertTextRules:
          monaco.languages.CompletionItemInsertTextRule.InsertAsSnippet,
      },
      {
        label: 'mul',
        kind: monaco.languages.CompletionItemKind.Function,
        insertText: 'oUNg ${1:var} OUng ${2:number} OUNg',
        insertTextRules:
          monaco.languages.CompletionItemInsertTextRule.InsertAsSnippet,
      },
      {
        label: 'div',
        kind: monaco.languages.CompletionItemKind.Function,
        insertText: 'oUNG ${1:var} OUng ${2:number} OUNg',
        insertTextRules:
          monaco.languages.CompletionItemInsertTextRule.InsertAsSnippet,
      },
      {
        label: 'print#',
        kind: monaco.languages.CompletionItemKind.Function,
        insertText: 'ouNg $0 OUNg',
        insertTextRules:
          monaco.languages.CompletionItemInsertTextRule.InsertAsSnippet,
      },
      {
        label: 'in',
        kind: monaco.languages.CompletionItemKind.Function,
        insertText: 'ounG Oung $0 Oung OUNg',
        insertTextRules:
          monaco.languages.CompletionItemInsertTextRule.InsertAsSnippet,
      },
      {
        label: 'or',
        kind: monaco.languages.CompletionItemKind.Keyword,
        insertText: 'OuNg ${1:var} OUng ${2:number} OUNg',
        insertTextRules:
          monaco.languages.CompletionItemInsertTextRule.InsertAsSnippet,
      },
      {
        label: 'and',
        kind: monaco.languages.CompletionItemKind.Keyword,
        insertText: 'OuNG ${1:var} OUng ${2:number} OUNg',
        insertTextRules:
          monaco.languages.CompletionItemInsertTextRule.InsertAsSnippet,
      },
    ];
    return { suggestions: suggestions };
  },
});

monaco.languages.setMonarchTokensProvider('oung', {
  defaultToken: 'default',
  tokenizer: {
    root: [
      [/ouNG|OuNg|OuNG/, 'keyword'],
      [/oung|OUNG/, 'number'],
      [/OunG|oUng|oUnG|oUNg|oUNG|ouNg|ounG/, 'function'],
      [/Oung/, 'string'],
      [/OUng/, 'sep'],
    ],
  },
});

monaco.editor.defineTheme('oung', {
  base: 'vs-dark',
  inherit: true,
  rules: [
    { token: 'default', foreground: 'FFFFFF' },
    { token: 'keyword', foreground: '7AB0FF' },
    { token: 'number', foreground: 'B7FF8F' },
    { token: 'function', foreground: 'C6AEFF' },
    { token: 'string', foreground: 'FFD771' },
    { token: 'sep', foreground: '6AE1DE' },
  ],
});
