// Copyright 2025 Google LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

package query

import (
	"context"
	"net/url"
	"strconv"
	"strings"

	"golang.org/x/exp/slices"
)

type queryContext struct {
	tzOffset          int
	supportedActions  []string
	preferredLanguage string
	preferredUnits    string
}

type qckt int

var queryContextKey qckt

func ContextWith(ctx context.Context, q url.Values) context.Context {
	offset, _ := strconv.Atoi(q.Get("tzOffset"))
	supportedActions := strings.Split(q.Get("actions"), ",")
	preferredLanguage := q.Get("lang")
	preferredUnits := q.Get("units")
	qc := queryContext{
		tzOffset:          offset,
		supportedActions:  supportedActions,
		preferredLanguage: preferredLanguage,
		preferredUnits:    preferredUnits,
	}
	ctx = context.WithValue(ctx, queryContextKey, qc)
	return ctx
}

func TzOffsetFromContext(ctx context.Context) int {
	return ctx.Value(queryContextKey).(queryContext).tzOffset
}

func SupportedActionsFromContext(ctx context.Context) []string {
	return ctx.Value(queryContextKey).(queryContext).supportedActions
}

func PreferredLanguageFromContext(ctx context.Context) string {
	return ctx.Value(queryContextKey).(queryContext).preferredLanguage
}

func PreferredUnitsFromContext(ctx context.Context) string {
	return ctx.Value(queryContextKey).(queryContext).preferredUnits
}

func SupportsAction(ctx context.Context, action string) bool {
	return slices.Contains(SupportedActionsFromContext(ctx), action)
}
