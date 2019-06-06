-- ����AI����

-- ����Ľű�����
-- 0����������뺰�е�tick
-- 1���������Ƿ񺰽����

-- �������ս������
function x1003001_OnEnterCombat(mapid, instanceid, creatureid)
	cre.SetCreatureAITimer(mapid, instanceid, creatureid, 5)
	cre.SetCreatureScriptData(mapid, instanceid, creatureid, 2, 0, 0, 1, 0)
end

-- �����뿪ս������
function x1003001_OnLeaveCombat(mapid, instanceid, creatureid)
	cre.SetCreatureAITimer(mapid, instanceid, creatureid, -1)
end

-- ����AI״̬���º���
function x1003001_OnUpdateAI(mapid, instanceid, creatureid, curstate)

	if 11 == curstate then	-- ���ں���״̬��
		-- �õ���ǰ�����������͹��￪ʼ����������
		local starttick = cre.GetCreatureScriptData(mapid, instanceid, creatureid, 1, 0)	--�õ���ʼ��е�tick
		local worldtick = aux.GetWorldTick()

		-- �������ʱ�䳬����3�룬����״̬����
		if nil ~= starttick and worldtick - starttick >= 20 then
			cre.CreatureChangeAIState(mapid, instanceid, creatureid, 1)	--�л���׷��״̬
		end

	else					-- �����ں���״̬��
		-- ������ﻹû�н��������״̬
		local entered = cre.GetCreatureScriptData(mapid, instanceid, creatureid, 1, 1)

		if 0 == entered then
			-- �õ��������ս���ķ����������͵�ǰ����������
			local entercombattick = cre.GetEnterCombatTick(mapid, instanceid, creatureid)
			local worldtick = aux.GetWorldTick()

			-- �������ս������1���ˣ����л������״̬
			if nil ~= entercombattick and worldtick - entercombattick >= 1 then
				cre.CreatureChangeAIState(mapid, instanceid, creatureid, 11)	--�л�������״̬
			end
		end
	end

end

-- �������AI״̬����
function x1003001_OnEnterCurAI(mapid, instanceid, creatureid, curstate)
	local ai_state_table = x1003001_AITable[curstate]

	if not ai_state_table then
		return
	end

	local fn = ai_state_table[1]

	if not fn then
		return
	end

	-- ���ú���
	fn(mapid, instanceid, creatureid)
end

-- �����뿪AI״̬����
function x1003001_OnLeaveCurAI(mapid, instanceid, creatureid, curstate)
	local ai_state_table = x1003001_AITable[curstate]

	if not ai_state_table then
		return
	end

	local fn = ai_state_table[2]

	if not fn then
		return
	end

	-- ���ú���
	fn(mapid, instanceid, creatureid)
end

-- �������AI״̬����
function x1003001_OnUpdateCurAI(mapid, instanceid, creatureid, curstate)
	local ai_state_table = x1003001_AITable[curstate]

	if not ai_state_table then
		return
	end

	local fn = ai_state_table[3]

	if not fn then
		return
	end

	-- ���ú���
	fn(mapid, instanceid, creatureid)
end

-- ������״̬���뺯��
local function x1003001_OnEnterCurAI_Shout(mapid, instanceid, creatureid)
	cre.SetCreatureScriptData(mapid, instanceid, creatureid, 2, 0, aux.GetWorldTick(), 1, 1)

	unit.StopAttack(mapid, instanceid, creatureid)

	cre.MonsterSay(mapid, instanceid, creatureid, 3)
end

-- ������״̬���º���
local function x1003001_OnUpdateCurAI_Shout(mapid, instanceid, creatureid)
	local starttick = cre.GetCreatureScriptData(mapid, instanceid, creatureid, 1, 0)	--�õ���ʼ��е�tick
	local worldtick = aux.GetWorldTick()

	local x, y, z = unit.GetPosition(mapid, instanceid, creatureid)
	local distance = math.pow(x-2200,2) + math.pow(z-2200,2)

	if distance<=100 then
		unit.StopMove(mapid, instanceid, creatureid, 1)
		cre.MonsterPlayerAction(mapid, instanceid, creatureid, "dfly")
	else
		cre.PullCreatureToPos(mapid, instanceid, creatureid, 2200, 6646, 2200)
	end
end

-- ������״̬�˳�����
local function x1003001_OnLeaveCurAI_Shout(mapid, instanceid, creatureid)
	unit.AddBuff(mapid, instanceid, creatureid, 9500601)
end

-- ����AI��
x1003001_AITable = {[11] = {x1003001_OnEnterCurAI_Shout, x1003001_OnLeaveCurAI_Shout, x1003001_OnUpdateCurAI_Shout}}

aux.RegisterCreatureEvent(1003001, 2, "x1003001_OnEnterCombat")
aux.RegisterCreatureEvent(1003001, 3, "x1003001_OnLeaveCombat")
aux.RegisterCreatureEvent(1003001, 8, "x1003001_OnUpdateAI")
aux.RegisterCreatureEvent(1003001, 9, "x1003001_OnUpdateCurAI")
aux.RegisterCreatureEvent(1003001, 10, "x1003001_OnEnterCurAI")
aux.RegisterCreatureEvent(1003001, 11, "x1003001_OnLeaveCurAI")
