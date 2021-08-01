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
    ];
    return { suggestions: suggestions };
  },
});

monaco.languages.setMonarchTokensProvider('oung', {
  defaultToken: 'default',
  tokenizer: {
    root: [
      [/ouNG|OUng/, 'keyword'],
      [/oung|OUNG/, 'number'],
      [/OunG/, 'print'],
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
    { token: 'print', foreground: 'C6AEFF' },
    { token: 'string', foreground: 'FFD771' },
    { token: 'sep', foreground: 'FFD771' },
  ],
});
