import * as monaco from 'monaco-editor';

monaco.languages.register({ id: 'oung' });

monaco.languages.registerCompletionItemProvider('oung', {
  provideCompletionItems: (model, position, token) => {
    const suggestions = [
      {
        label: 'byte',
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
        label: '0',
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
        label: '1',
        kind: monaco.languages.CompletionItemKind.Snippet,
        insertText: 'OUNG',
        documentation: '1 is 1',
      },
      {
        label: 'equal',
        kind: monaco.languages.CompletionItemKind.Snippet,
        insertText: 'ouNg',
        documentation: '=',
      },
      {
        label: 'print',
        kind: monaco.languages.CompletionItemKind.Function,
        insertText: 'OunG',
        documentation: '=',
      },
      {
        label: 'semi',
        kind: monaco.languages.CompletionItemKind.Snippet,
        insertText: 'OUNg',
        documentation: '=',
      },
    ];
    return { suggestions: suggestions };
  },
});

monaco.languages.setMonarchTokensProvider('oung', {
  defaultToken: 'default',
  tokenizer: {
    root: [
      [/ouNG/, 'byte'],
      [/ouNg/, 'equal'],
      [/oung|OUNG/, 'number'],
      [/OunG/, 'print'],
    ],
  },
});

monaco.editor.defineTheme('oung', {
  base: 'vs-dark',
  inherit: true,
  rules: [
    { token: 'default', foreground: 'FFFFFF' },
    { token: 'byte', foreground: '7AB0FF' },
    { token: 'number', foreground: 'B7FF8F' },
    { token: 'print', foreground: 'c6aeff' },
  ],
});
